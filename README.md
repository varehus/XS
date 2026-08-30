XS: The eXtensible Shell
========================

This is a maintained fork of xs.

XS is the eXtensible Shell, a command line interpreter featuring
Lisp-like semantics (lists, function parameters, closures, exceptions,
lexical bindings, lambdas, etc.) and a conventional syntax.

### Changelog
* For recent updates under new maintenance, see the root [ChangeLog](ChangeLog).
* For historical changes prior to version 1.3, see the legacy [doc/CHANGES](doc/CHANGES).

### Testing

* Run the test harness using `./run-tests.sh` (portable).
* Run the fuzz test harness using `./run-fuzz.sh` (requires systemd virtualization).

---

## About / Original Documentation

This is `xs 1.3`; see `doc/CHANGES`.

The package includes extensive documentation. Sample scripts to define
library functions, tools and utilities are found in
[https://github.com/TieDyedDevil/XS-library.git](https://github.com/TieDyedDevil/XS-library.git).
The snapshot at [https://github.com/vaerksted/XS.git](https://github.com/vaerksted/XS.git)
includes `samples/_xsrc.d/annotate.xs` as referenced in [doc/PROJECTS](doc/PROJECTS).
