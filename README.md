# egt-samples-contribution

This repository is dedicated to EGT contributions with a light process for
inclusion. The code won't be reviewed or maintained by the EGT team. Some
examples may be moved from this repository to an official EGT repository. In
this case, the EGT team will review the code and will be responsible for its
maintenance.

Examples from this repository may be distributed within linux4sam if they work
at the time of the release.

Here are the rules to submit code:

- Create a folder for your code.

- Code must use the Apache 2.0 License

- Using a build system as CMake or autotools is strongly recommended. At least,
a Makefile is required.

- Code must compile without any warnings with `-Wall -Wextra`

- A README file is required. Here is the pattern:
````
# Example name

## Description

A new shiny widget.

## EGT version

tag or commit id

## Test

- PC (Ubuntu 20.04)
- sam9x60ek (linux4sam-2021.04)

## Dependencies

- libegt
- libcairo
- libfoo

## Compilation Instructions

./autogen.sh
./configure
make

## Screenshot

![](screenshot.png)

````

- The screenshot.png file is required. It can be generated sending `SIGUSR2` signal
to your application.
````
    examples/widgets/widgets &

    [1] 25437

    kill -s SIGUSR2 25437
````

- Submit the pull request.
- A Tested-by tag by one of the member of the EGT team is required to merge the
  pull request.

