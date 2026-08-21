project('PSVR2PollRateFixer', 'cpp', default_options : ['cpp_std=c++26'])

if get_option('debug')
    add_project_arguments('-DDEBUG', language: 'cpp')
endif

executable(
    'PSVR2PollRateFixer',
    sources: [
        'src/CRC32.cpp',
        'src/Application.cpp',
        'src/main.cpp'
    ],
    include_directories: include_directories('include'),
    dependencies: [
        dependency('hidapi-hidraw')
    ]
)

