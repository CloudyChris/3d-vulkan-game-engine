// ProjectFilter (vulkan)

#include "cm_commons.hpp"

class CC_ShaderCompiler
{
public:
  CC_ShaderCompiler(const std::string aConfigFile);
  std::string PreprocessShader(const std::string& source_name, shaderc_shader_kind kind, const std::string& source);
  // TODO: consider if compiling to asm should automatically happen for debug and if it's needed for the release build at all
  std::string CompileFileToSpirVAsm(const std::string& source_name, shaderc_shader_kind kind, const std::string& source, bool optimize = false);
  std::vector<uint32_t> CompileFileToSpirVBin(const std::string& source_name, shaderc_shader_kind kind, const std::string& source, bool optimize = false);
};
