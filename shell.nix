{
  pkgs ? import <nixpkgs> {},
  lib ? pkgs.lib,
}:
with pkgs;
  mkShell {
    name = "claw";
    packages = [
      cmake
      jansson
      zlib
      libuv
      openssl
      pkg-config
      file
      bear
      gcc
      clang-tools
      wslay
      brotli
      libcap
      valgrind
      libgit2
    ];

    shellHook = ''
      mkdir -p ./tmp
      export TMPDIR=$(realpath ./tmp)
    '';
  }
