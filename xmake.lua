add_rules("mode.debug", "mode.release")

set_languages("c++17")
add_requires("conan::boost/1.86.0", {alias = "boost"})

-- library
target("rawnet")
    add_packages("boost")
    set_kind("static")
    add_files("src/*.cpp")
    add_headerfiles("src/*.hpp")

-- test
target("rawnet-test")
    add_packages("boost")
    set_kind("binary")
    add_files("test/*.cpp")
    add_deps("rawnet")
    add_linkdirs("$(buildir)")
    add_links("rawnet")
    add_includedirs("src")
