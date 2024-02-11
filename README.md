# NidhoggScript

<p align="center">
  <img alt="Logo" src="./images/logo.png">
</p>

![image](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white) ![image](https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white)

NidhoggScript is a project that allows the user to create playbooks and execute a bunch of commands with Nidhogg in one IOCTL. This project has two parts:

- This repository, which is the user mode part contains the user mode "assembler" and checker to create these script files.

- [Nidhogg](https://github.com/Idov31/Nidhogg) that contains the execution part.

The allowed commands and detailed usage will be documented in a dedicated wiki soon.

## Basic Usage

Once you compiled the project, you can run it via CLI in two ways:

### Assembling mode

Use this mode to "assemble" your script, give the input file and the output directory which will be created there the `out.ndhg` file.

```sh
# Example input in file.txt:
# process add 3110

# Run the assembler.
NidhoggScript.exe C:\path\to\your\file.txt C:\path\to\folder
```

### Validation mode

Use this mode if you want to validate an edited file. If it is a file that is just generated there is no need to use this mode because "assembling" also validates the output.

```sh
NidhoggScript.exe C:\path\to\your\file.ndhg
```

## Setup

To compile the project, you will need to have [Visual Studio 2022](https://visualstudio.microsoft.com/thank-you-downloading-visual-studio/?sku=Community&rel=16) installed and then just build the project like any other Visual Studio project.
