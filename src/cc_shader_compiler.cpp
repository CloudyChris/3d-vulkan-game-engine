// ProjectFilter (vulkan)

#include "cm_precompile.hpp"
#include "cc_shader_compiler.hpp"

CC_ShaderCompiler::CC_ShaderCompiler(const std::string aConfigFile)
{
  return;
}

std::string CC_ShaderCompiler::PreprocessShader(const std::string& source_name, shaderc_shader_kind kind, const std::string& source)
{
  shaderc::Compiler compiler;
  shaderc::CompileOptions options;

  // Like -DMY_DEFINE=1
  // TODO: remove this or use the syntax for config
  options.AddMacroDefinition("MY_DEFINE", "1");

  shaderc::PreprocessedSourceCompilationResult result = compiler.PreprocessGlsl(source, kind, source_name.c_str(), options);

  if (result.GetCompilationStatus() != shaderc_compilation_status_success) {
    // TODO: consider soft asserting this or hard assert depening on the build (debug should hard assert)
    std::cerr << result.GetErrorMessage();
    return "";
  }

  return {result.cbegin(), result.cend()};
}

std::string CC_ShaderCompiler::CompileFileToSpirVAsm(const std::string& source_name, shaderc_shader_kind kind, const std::string& source, bool optimize)
{
  shaderc::Compiler compiler;
  shaderc::CompileOptions options;

  // Like -DMY_DEFINE=1
  // TODO: remove this or use the syntax for config
  options.AddMacroDefinition("MY_DEFINE", "1");
  // TODO: optimize and optimization level per shader should be specified in a build list generated automagically for all registered shaders
  // a shader registry. will also help with debugging
  if (optimize) options.SetOptimizationLevel(shaderc_optimization_level_performance);

  shaderc::AssemblyCompilationResult result = compiler.CompileGlslToSpvAssembly(source, kind, source_name.c_str(), options);

  if (result.GetCompilationStatus() != shaderc_compilation_status_success)
  {
    // TODO: consider soft asserting this or hard assert depening on the build (debug should hard assert)
    std::cerr << result.GetErrorMessage();
    return "";
  }

  return {result.cbegin(), result.cend()};
}

std::vector<uint32_t> CC_ShaderCompiler::CompileFileToSpirVBin(const std::string& source_name, shaderc_shader_kind kind, const std::string& source, bool optimize)
{
  shaderc::Compiler compiler;
  shaderc::CompileOptions options;

  // Like -DMY_DEFINE=1
  // TODO: remove this or use the syntax for config
  options.AddMacroDefinition("MY_DEFINE", "1");
  // TODO: optimize and optimization level per shader should be specified in a build list generated automagically for all registered shaders
  // a shader registry. will also help with debugging
  if (optimize) options.SetOptimizationLevel(shaderc_optimization_level_size);

  shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, kind, source_name.c_str(), options);

  if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
    // TODO: consider soft asserting this or hard assert depening on the build (debug should hard assert)
    std::cerr << module.GetErrorMessage();
    return std::vector<uint32_t>();
  }

  return {module.cbegin(), module.cend()};
}
