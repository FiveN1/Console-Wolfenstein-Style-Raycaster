workspace "Console_Raycaster"
    architecture "x86_64"
	startproject "CSource"

    configurations {
        "Debug",
        "Release"
    }

    flags {
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}" 

project "Console_Raycaster"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
    staticruntime "on"
	-- compile dir
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"src/**.h",
		"src/**.cpp",
		-- stb image fork
		"vendor/stb/include/stb/stb_image_imp.h",
        "vendor/stb/include/stb/stb_image_imp.cpp"
	}
	-- pre-compiled header set
	pchheader "pch.h"
	pchsource "src/pch.cpp"
	--
	includedirs {
		"src",
		"vendor/stb/include"
	}

	filter "system:windows"
		systemversion "latest"
		defines {
			"CFG_PLATFORM_WINDOWS" -- přidat aby premake vytvořil .h soubor se všemi definicemi pro kompatibilitu s vscode
		}
	filter "configurations:Debug" 
		defines "CFG_DEBUG"
		symbols "on"
	filter "configurations:Release" 
		defines "CFG_RELEASE"
		optimize "on"


newaction {
    trigger = "clean",
    description = "Remove all binaries and intermediate binaries, and vs files.",
    execute = function()
        print("Removing binaries")
        os.rmdir("./bin")
        print("Removing intermediate binaries")
        os.rmdir("./bin-int")
        print("Removing project files")
        os.rmdir("./.vs")
        os.remove("**.sln")
        os.remove("**.vcxproj")
        os.remove("**.vcxproj.filters")
        os.remove("**.vcxproj.user")
        print("Done")
    end
}