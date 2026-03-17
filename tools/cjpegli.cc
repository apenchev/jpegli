// Copyright (c) the JPEG XL Project Authors.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

#include "lib/jpegli/encode.h"

void MyErrorExit(j_common_ptr cinfo) {
  (*cinfo->err->output_message)(cinfo);
  jpegli_destroy_compress(reinterpret_cast<j_compress_ptr>(cinfo));
}

unsigned long size_arr[1];

extern "C" {

unsigned char* encode(
  unsigned char* input_buffer,
  int width,
  int height,
  int chroma_subsampling,
  int quality,
  int progressive_level,
  bool optimize_coding,
  bool use_adaptive_quantization,
  bool use_std_quant_tables
) {
  unsigned char* output_buffer = nullptr;
  unsigned long output_size = 0;

  jpeg_compress_struct cinfo;
  jpeg_error_mgr jerr;
  cinfo.err = jpegli_std_error(&jerr);
  jerr.error_exit = &MyErrorExit;

  jpegli_create_compress(&cinfo);
  jpegli_mem_dest(&cinfo, &output_buffer, &output_size);
  cinfo.image_width = width;
  cinfo.image_height = height;
  cinfo.input_components = 4;
  cinfo.in_color_space = JCS_EXT_RGBA;
  if (use_std_quant_tables) {
    jpegli_use_standard_quant_tables(&cinfo);
  }

  jpegli_set_defaults(&cinfo);
  if (chroma_subsampling == 444) {
    cinfo.comp_info[0].h_samp_factor = 1;
    cinfo.comp_info[0].v_samp_factor = 1;
  } else if (chroma_subsampling == 440) {
    cinfo.comp_info[0].h_samp_factor = 1;
    cinfo.comp_info[0].v_samp_factor = 2;
  } else if (chroma_subsampling == 422) {
    cinfo.comp_info[0].h_samp_factor = 2;
    cinfo.comp_info[0].v_samp_factor = 1;
  } else if (chroma_subsampling == 420) {
    cinfo.comp_info[0].h_samp_factor = 2;
    cinfo.comp_info[0].v_samp_factor = 2;
  }
  for (int i = 1; i < cinfo.num_components; ++i) {
    cinfo.comp_info[i].h_samp_factor = 1;
    cinfo.comp_info[i].v_samp_factor = 1;
  }

  jpegli_enable_adaptive_quantization(&cinfo, use_adaptive_quantization);
  float distance = jpegli_quality_to_distance(quality);
  jpegli_set_distance(&cinfo, distance, 1);
  jpegli_set_progressive_level(&cinfo, progressive_level);
  cinfo.optimize_coding = optimize_coding;

  jpegli_set_input_format(&cinfo, JPEGLI_TYPE_UINT8, JPEGLI_NATIVE_ENDIAN);
  jpegli_start_compress(&cinfo, 1);

  // Copyright (c) 2024, gen2brain
  // https://github.com/gen2brain/jpegli/blob/main/LICENSE
  JSAMPROW row[1];
  int stride = cinfo.image_width * cinfo.input_components;
  while (cinfo.next_scanline < cinfo.image_height) {
    row[0] = &input_buffer[cinfo.next_scanline * stride];
    jpegli_write_scanlines(&cinfo, row, 1);
  }

  jpegli_finish_compress(&cinfo);
  jpegli_destroy_compress(&cinfo);

  size_arr[0] = output_size;
  return output_buffer;
}

unsigned long get_size() {
  return size_arr[0];
}

} // extern "C"