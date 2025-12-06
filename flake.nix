{
	description = "keel-os build environment flake";

	inputs = {
		nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
	};

	outputs = {
		self,
		nixpkgs,
	}: let
		pkgs = nixpkgs.legacyPackages.x86_64-linux;
	in {
		devShells.x86_64-linux.default =
			pkgs.mkShell {
				buildInputs = with pkgs; [
					binutils
					gcc_multi
					glibc_multi
					gnumake
					nasm
					qemu_full
				];
			};
	};
}
