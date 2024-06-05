# Orchid compiler frontend
![Lines Of Code Badge](https://raw.githubusercontent.com/orchid-lang/frontend/image-data/cloc_badge.svg)
[![License: MPL 2.0](https://img.shields.io/badge/License-MPL_2.0-brightgreen.svg)](https://opensource.org/licenses/MPL-2.0)
![GitHub commit activity](https://img.shields.io/github/commit-activity/w/orchid-lang/frontend)

This is the compiler frontend for the orchid language.

## About

The frontend responsibilities are to:

1. Tokenize a file
2. Parse the tokens into an AST
3. Analyze the AST for errors
4. Optimize the AST to not recompute constants

## Contributing

Contributions are always welcome!

Here is a step-by-step guide on how you can contribute!

1. Read the [`CONTRIBUTING`](CONTRIBUTING.md) file so you know what you want do is actually something that is wanted or possible.
2. Fork the repo.
3. Create a new branch. (`git checkout -b feature`)
4. Make, commit, and push your changes.
5. Create a pull request with your changes!

## Building

This is a guide to just build the frontend.

### Windows

The recommended way is to use [Microsoft Visual Studio 2022](https://visualstudio.microsoft.com/vs/) (You can probably use any version but that is what it was made with).

From there you can open the project, then use the build in build tools.

The build tools should be in the bar on the top of the screen :)

### Linux

To compile the code on linux (or without visual studio) you need to have g++ 13 or later installed.

on debian based systems you can install it like this:

```bash
sudo apt update
sudo apt install g++-13
```

You also need to have cmake installed, this can be done on debian based distros with
```bash
sudo apt install cmake
```

To build the project you can make a directory called `out`, enter it, then run cmake.
```bash
mkdir out
cd out
cmake ..
```

Then you can build the project to manually test with make.
```bash
make frontend
```

You can build the tests with 
```bash
make lexer_tests
```

Then run them with
```bash
make test
```

You can view all the make targets if needed with `make help`.

## License

The orchid compiler frontend is licensed under the Mozilla Public License 2.0 (MPL 2.0).

[view the full license](LICENSE.txt)

