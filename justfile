build_dir := "build"
build_bin_dir := build_dir / "bin"
build_script := build_dir / "build.sh"

@default:
  just --list

@build-libh2o:
  {{build_script}} -ih2o

@get-h2o-headers:
  {{build_script}} -hh2o

@compile:
  {{build_script}} -c

@link:
  {{build_script}} -l

@build-all:
  just build-libh2o
  just build
  mv {{build_bin_dir}}/toast .

@build:
  just compile
  just link

@run:
  just build
  {{build_bin_dir}}/claw

@generate-compilation-database:
  {{build_script}} -cd
