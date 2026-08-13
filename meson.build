project('xs', ['cpp'],
  version: '1.3',
  meson_version: '>= 1.11.1'
)
  compiler = meson.get_compiler('cpp')
  compile_flags = ['-I../src', '-Wall', '-Wextra']
  link_flags = []
  boost_dep = dependency('boost', version: '>=1.78')
  ffi_lib = dependency('libffi', version: '>=3.3', method: 'auto', required: true)
  gc_lib = dependency('bdw-gc', 'gc', version: '>=8.0.6', method: 'auto', required: true)
  gccpp_lib = compiler.find_library('gccpp')
  readline_lib = dependency('readline', version: '>=8.1', method: 'auto', required: true)
buildinfo_script = find_program('generators/buildinfo.sh')
git_date_script  = find_program('generators/git_date.sh')
git_hash_script  = find_program('generators/git_hash.sh')
git_url_script   = find_program('generators/git_url.sh')
initial_script   = find_program('generators/initial.sh')
mksignal_script  = find_program('generators/mksignal.sh')
  custom_target('.stamp',
	build_always_stale: true,
	build_by_default: true,
	output: '.stamp',
	command: ['touch', '@OUTPUT@'])
  custom_target('buildinfo.hxx',
	build_always_stale: true,
	build_by_default: true,
	output: 'buildinfo.hxx',
	command: [buildinfo_script])
  custom_target('git_date.hxx',
	build_always_stale: true,
	build_by_default: true,
	output: 'git_date.hxx',
	command: [git_date_script])
  custom_target('git_hash.hxx',
	build_always_stale: true,
	build_by_default: true,
	output: 'git_hash.hxx',
	command: [git_hash_script])
  custom_target('git_url.hxx',
	build_always_stale: true,
	build_by_default: true,
	output: 'git_url.hxx',
	command: [git_url_script])
  parse_cxx = custom_target('parse.cxx',
	depend_files: 'src/parse.yxx',
	input: ['src/parse.yxx'],
	output: ['parse.tab.cxx', 'parse.tab.hxx'],
	command: ['bison', '-d', '@INPUT@'])
  sigmsgs_cxx = custom_target('sigmsgs.cxx',
	output: 'sigmsgs.cxx',
	command: [mksignal_script, '@OUTPUT@'])
  common_sources = ['src/access.cxx', 'src/closure.cxx', 'src/conv.cxx',
		'src/eval.cxx', 'src/fd.cxx', 'src/glob.cxx', 'src/glom.cxx',
		'src/heredoc.cxx', 'src/input.cxx', 'src/list.cxx',
		'src/main.cxx', 'src/match.cxx', 'src/opt.cxx', parse_cxx,
		'src/prim-ctl.cxx', 'src/prim.cxx', 'src/prim-etc.cxx',
		'src/prim-io.cxx', 'src/prim-rel.cxx', 'src/prim-sys.cxx',
		'src/print.cxx', 'src/proc.cxx', sigmsgs_cxx, 'src/signal.cxx',
		'src/split.cxx', 'src/status.cxx', 'src/str.cxx',
		'src/syntax.cxx', 'src/term.cxx', 'src/token.cxx',
		'src/tree.cxx', 'src/util.cxx', 'src/var.cxx',
		'src/version.cxx', 'src/buildinfo.cxx']
  common_dependencies = [boost_dep, readline_lib, gc_lib, gccpp_lib, ffi_lib]
  xsdump = executable('xsdump', ['src/dump.cxx', common_sources],
	cpp_args: compile_flags,
	dependencies: common_dependencies)
  initial_cxx = custom_target('initial.cxx',
	input: 'src/initial.xs',
	output: 'initial.cxx',
	depends: xsdump,
	command: [initial_script, '@INPUT@', '@OUTPUT@'])
  xs = executable('xs', [initial_cxx, common_sources],
	cpp_args: compile_flags,
	link_args: link_flags,
	dependencies: common_dependencies,
	install: true)
  install_man(['man/xs.1'])
  install_data(['doc/ANCESTORS', 'doc/CHANGES', 'doc/COPYING',
		'INSTALL', 'doc/PHILOSOPHY', 'doc/PROJECTS', 'README.md',
		'doc/SYSTEMD_LOCALE', 'doc/TODO', 'doc/TUTORIAL.md',
		'doc/XS-FOR-LISPERS.md', 'xs.lang'],
	install_dir: 'share/doc/xs')
