project('kite', 'c',
  version : '1.0.0',
  license : 'Apache-2.0'
)

dirs = include_directories(
  'src',
)

source  = '*.c'
headers = '*.h'
bin     = '/usr/bin/kite'

cflags = []
cflags += [
  '-g', '-std=c99'
]

executable(bin, source,
  install : true,
  c_args : cflags,
  include_directories : dirs
)
