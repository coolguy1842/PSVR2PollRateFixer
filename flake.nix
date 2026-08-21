{
    inputs = {
        nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
        flake-utils.url = "github:numtide/flake-utils";
    };

    outputs = { self, nixpkgs, flake-utils, ... }:
        flake-utils.lib.eachDefaultSystem (system: let
            pkgs = import nixpkgs { inherit system; config.allowUnfree = true; };
        in {
            devShells.default = pkgs.mkShell {
                hardeningDisable = [ "all" ];
                nativeBuildInputs = with pkgs; [
                    clang-tools
                    
                    llvmPackages_latest.lldb
                    llvmPackages_latest.libllvm
                    llvmPackages_latest.libcxx
                    llvmPackages_latest.clang

                    pkg-config
                    ninja
                    cmake
                    meson
                ];

                buildInputs = with pkgs; [ hidapi ];
            };

        packages = rec {
            PSVR2PollRateFixer = pkgs.stdenv.mkDerivation {
                name = "PSVR2PollRateFixer";
                version = "1.0.0";
                src = ../.;

                nativeBuildInputs = with pkgs; [
                    clang
                    pkg-config
                    ninja
                    cmake
                    meson
                ];

                buildInputs = with pkgs; [ hidapi ];

                configurePhase = ''
                    meson setup build -Dprefix=$out -Dbuildtype=release
                '';

                buildPhase = ''
                    meson compile -C build
                '';

                installPhase = ''
                    mkdir -p $out/bin
                    cp build/PSVR2PollRateFixer $out/bin/
                '';

                meta = with pkgs.lib; {
                    description = "Changes the poll-rate on PSVR2 controllers automatically";
                    homepage = "https://github.com/coolguy1842/PSVR2PollRateFixer/";
                    license = licenses.gpl3;
                };
            };

            default = PSVR2PollRateFixer;
        };
    });
}