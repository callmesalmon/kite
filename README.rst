Kite Programming Language
============================
.. image:: https://img.shields.io/badge/Build%20(fedora)-passing-2a7fd5?logo=fedora&logoColor=2a7fd5&style=for-the-badge
   :alt: Build = Passing
   :target: https://github.com/ElisStaaf/kite
.. image:: https://img.shields.io/badge/Version-1.0.0-38c747?style=for-the-badge
   :alt: Version = 1.0.0
   :target: https://github.com/ElisStaaf/kite
.. image:: https://img.shields.io/badge/Language-C-grey?logo=c&logoColor=white&labelColor=blue&style=for-the-badge
   :alt: Language = C
   :target: https://github.com/ElisStaaf/kite

Kite is a pure and minimalistic programming language written in C.
This is a sample of what code written in Kite might look like:

.. code:: coffeescript

   (let output = 0)
   (let i = 1)
   (while i < 10 do
       (let j = 0)

       (while j < 10 do
           (output = (output + j * i))
           (if j == 5 then break)
           (j = (j + 1))
       end)
       (i = (i + 1))
   end)

Simple program to print the number ``100``:

.. code:: coffeescript
   
   # To print without newline:
   (write(tostring(100)))

   # or

   (import io)
   (io.print(100))

   # To print with a newline:
   (writeln(tostring(100)))

   # or

   (import io)
   (io.println(100))

It also includes functions:

.. code:: coffeescript

   (let println = (fun(v)
       (writeln(tostring(v)))))

   (let sum = (fun(x)
       (if x < 2 then (x))
       (else (x + sum(x - 1)))))

   (println(sum(3)))

This does the same thing as the last example:

.. code:: coffeescript

   (let println = (fun(v)
       (writeln(tostring(v)))))

   (println((fun(x) if x < 2 then (x) else (x + @(x - 1)))(3)))

Oh, almost forgot about Modules:

.. code:: coffeescript

   (import io)
   (io.print(10))
   (io.println(20))
   (io.print(30))

stdlib/io.kite:

.. code:: coffeescript

   (let export print = (fun(val) (write(tostring(val)))))
   (let export println = (fun(val) (writeln(tostring(val)))))

(All modules should be located in ./stdlib/)

Requirements
------------
* `gcc`_ (or any other C compiler) 
* `make`_ or `meson`_
* `git`_ or `gh`_

Installation
------------
To install, firstly clone the repo:

.. code:: sh

   # git
   git clone https://github.com/ElisStaaf/kite

   # gh
   gh repo clone ElisStaaf/kite

Then build an executable using make:

.. code:: sh

   cd kite

   # make
   sudo make

   # meson
   meson setup build && cd build
   meson compile

.. _`gcc`: https://gcc.gnu.org/install
.. _`make`: https://www.gnu.org/software/make
.. _`meson`: https://mesonbuild.com/Getting-meson.html
.. _`git`: https://git-scm.com/downloads 
.. _`gh`: https://github.com/cli/cli#installation
