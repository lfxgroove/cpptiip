project('cpptiip', 'cpp', default_options: ['cpp_std=c++11'])
boost_dep = dependency('boost', modules:  ['variant', 'uuid'])

subproject('cpplibutil')
util_dep = dependency('cpplibutil', fallback: ['cpplibutil', 'util_dep'])

subproject('js0n')
js0n_dep = dependency('js0n', fallback: ['js0n', 'js0n_dep'])

sources = ['tiip.cpp']
test_sources = ['tests/tiip.cpp', 'tests/test_main.cpp']

tiip_inc = include_directories('./')
tiip_dep = declare_dependency(sources: sources, dependencies: [js0n_dep, util_dep, boost_dep], include_directories: tiip_inc, version: '1.0')

install_headers('tiip.h')

if not meson.is_subproject()
    executable('tests', test_sources + sources, dependencies: [js0n_dep, util_dep, boost_dep])
endif
