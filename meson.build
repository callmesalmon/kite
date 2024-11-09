project( 'kite', 'c',
  version : '0.1',
  license : 'Apache-2.0'
)

source  = 'src/*.c'
headers = 'src/*.h'
bin     = '/usr/bin/kite'

cflags = []
cflags += [
  '-g', '-std=c99'
]

executable(bin, source,
  install : true,
  c_args : cflags
)
