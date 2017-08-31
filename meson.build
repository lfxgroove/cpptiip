project('cpptiip', 'cpp', default_options: ['cpp_std=c++1z'])
boost_dep = dependency('boost', modules:  ['variant', 'uuid'])

subproject('cpplibutil')
util_dep = dependency('cpplibutil', fallback: ['cpplibutil', 'util_dep'])

subproject('js0n')
js0n_dep = dependency('js0n', fallback: ['js0n', 'js0n_dep'])

sources = ['tiip.cpp']
test_sources = ['tests/tiip.cpp', 'tests/test_main.cpp']

executable('tests', test_sources + sources, dependencies: [js0n_dep, util_dep, boost_dep])