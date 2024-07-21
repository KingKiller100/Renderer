import os

from conan import ConanFile
from conan.tools.microsoft import MSBuildToolchain, MSBuildDeps, MSBuild, vs_layout
from conan.tools.files import copy


class gfxConan(ConanFile):
	name = "gfx"
	version = "0.1.0"

	# Binary configuration
	package_type = "static-library" # hardcoded in .vcxproj
	settings = "os", "compiler", "build_type", "arch"

	# Sources are located in the same place as this recipe, copy them to the recipe
	exports_sources = "Gfx.sln", "src/*"
	options = {
		"api": ["opengl"]
	}
	default_options={
		"api": "opengl"
	}

	def layout(self):
		vs_layout(self)

	def requirements(self):
		self.requires("glfw/3.4@#556ce0bc9e0abc75fe4378b86e566fc4")

	def generate(self):
		MSBuildDeps(self).generate()
		tc = MSBuildToolchain(self)
		tc.generate()

	def build(self):
		msbuild = MSBuild(self)
		msbuild.build("gfx.sln")

	def package(self):
		from os.path import join
		if self.options.api == "opengl":
			copy(self, "opengl.lib", src=join(self.build_folder, self.settings.build_type), dst=join(self.package_folder, "lib"))
			copy(self, "opengl.pdb", src=join(self.build_folder, self.settings.build_type), dst=join(self.package_folder, "lib"))

	def package_info(self):
		if self.options.api == "opengl":
			self.cpp_info.libs = ["opengl"]
