#include <cstdint>
#include <cstdlib>

#include "lib/base/types.h"
#include "lib/jpegli/encode.h"

struct WasmJpegSettings {
  int chroma_subsampling;
  int quality;
  int progressive_level;
  bool optimize_coding;
  bool use_adaptive_quantization;
  bool use_std_quant_tables;
};

void MyErrorExit(j_common_ptr cinfo) {
  (*cinfo->err->output_message)(cinfo);
  jpegli_destroy_compress(reinterpret_cast<j_compress_ptr>(cinfo));
}

size_t size_arr[1];

extern "C" {

uint8_t* encode(
  uint8_t* input_buffer,
  int width,
  int height,
  int chroma_subsampling,
  int quality,
  int progressive_level,
  bool optimize_coding,
  bool use_adaptive_quantization,
  bool use_std_quant_tables
) {
  WasmJpegSettings jpeg_settings = {
    chroma_subsampling,
    quality,
    progressive_level,
    optimize_coding,
    use_adaptive_quantization,
    use_std_quant_tables
  };

  uint8_t* output_buffer;
  size_t output_size;

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
  if (jpeg_settings.use_std_quant_tables) {
    jpegli_use_standard_quant_tables(&cinfo);
  }

  jpegli_set_defaults(&cinfo);
  if (jpeg_settings.chroma_subsampling == 444) {
    cinfo.comp_info[0].h_samp_factor = 1;
    cinfo.comp_info[0].v_samp_factor = 1;
  } else if (jpeg_settings.chroma_subsampling == 440) {
    cinfo.comp_info[0].h_samp_factor = 1;
    cinfo.comp_info[0].v_samp_factor = 2;
  } else if (jpeg_settings.chroma_subsampling == 422) {
    cinfo.comp_info[0].h_samp_factor = 2;
    cinfo.comp_info[0].v_samp_factor = 1;
  } else if (jpeg_settings.chroma_subsampling == 420) {
    cinfo.comp_info[0].h_samp_factor = 2;
    cinfo.comp_info[0].v_samp_factor = 2;
  }
  for (int i = 1; i < cinfo.num_components; ++i) {
    cinfo.comp_info[i].h_samp_factor = 1;
    cinfo.comp_info[i].v_samp_factor = 1;
  }

  jpegli_enable_adaptive_quantization(
      &cinfo, TO_JXL_BOOL(jpeg_settings.use_adaptive_quantization));
  float distance = jpegli_quality_to_distance(jpeg_settings.quality);
  jpegli_set_distance(&cinfo, distance, TRUE);
  jpegli_set_progressive_level(&cinfo, jpeg_settings.progressive_level);
  cinfo.optimize_coding = TO_JXL_BOOL(jpeg_settings.optimize_coding);

  jpegli_set_input_format(&cinfo, JPEGLI_TYPE_UINT8, JPEGLI_NATIVE_ENDIAN);
  jpegli_start_compress(&cinfo, TRUE);

  JSAMPROW row[1];
  int stride = cinfo.image_width * cinfo.input_components;
  while (cinfo.next_scanline < cinfo.image_height) {
    row[0] = &input_buffer[cinfo.next_scanline * stride];
    jpegli_write_scanlines(&cinfo, row, 1);
  }

  jpegli_finish_compress(&cinfo);
  jpegli_destroy_compress(&cinfo);
  if (input_buffer) free(input_buffer);

  size_arr[0] = output_size;
  return output_buffer;
}

size_t get_size() {
  return size_arr[0];
}

} // extern "C"