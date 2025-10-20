-- premake5.lua
workspace "BinEncoding"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "BinEncoding"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

include "BinEncoding/Build-BinEncoding.lua"