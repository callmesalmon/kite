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
It can be a lot, but if I'd *have* to classify it, it's a functional
programming language with imperative elements. This is a sample of what
code written in Kite might look like:

.. code:: coffeescript

   let loop = fn(func, times, until)
       if times < until then @(func, times + 1, until)
       else func

   let incr = fn(num)
       num + 1

   let mainloop = fn(k, j, i)
       loop(incr(k), j, i)

   loop(mainloop(1, 5, 25), 0, 10)

Simple program to print the number ``100``:

.. code:: coffeescript
   
   # To print without newline:
   write(tostring(100)
   # or
   import io
   io.print(100)

   # To print with a newline:
   writeln(tostring(100))
   # or
   import io
   io.println(100)

It also includes functions:

.. code:: coffeescript

   let println = fn(v)
       writeln(tostring(v)

   let sum = fn(x)
       if x < 2 then x
       else x + sum(x - 1)

   println(sum(3))

This does the same thing as the last example:

.. code:: coffeescript

   let println = fn(v)
       writeln(tostring(v))

   println((fn(x) if x < 2 then x else x + @(x - 1))(3))

Oh, almost forgot about Modules:

.. code:: coffeescript

   import io
   io.print(10)
   io.println(20)
   io.print(30)

stdlib/io.kite:

.. code:: coffeescript

   let export print = fn(val)
       write(tostring(val))
   let export println = fn(val)
       writeln(tostring(val))

Another module; func:

.. code:: coffeescript

   import func

   let test = fn()
       writeln(tostring(100))

    func.loop(test(), 0, 10)

stdlib/func.kite:

.. code:: coffeescript

   let export loop = fn(func, times, until)
       if times < until then @(func, times + 1, until)
       else func

(All modules should be located in ./stdlib/)

Requirements
------------
* `gcc`_ (or any other C compiler) 
* `make`_
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

   sudo make

.. _`gcc`: https://gcc.gnu.org/install
.. _`make`: https://www.gnu.org/software/make
.. _`git`: https://git-scm.com/downloads 
.. _`gh`: https://github.com/cli/cli#installation
