# Invoice CLI
Invoice-CLI is a versatile C++ CLI application that leverages [OpenXLSX](https://github.com/troldal/OpenXLSX) and [Boost](https://www.boost.org/) libraries to generate, manage, and email invoices, allowing seamless batch writing to Excel files, exporting to PDFs, and provides a user-friendly command-line interface to meet all invoicing needs efficiently.
> **Note:** This project is for my personal use and is not intended for public use. However, if you find it useful, feel free to use it.

## Installation
Download the latest release from the [releases]().

## Usage
```shell
./invoice --help
```

The invoice CLI requires a command and a set of options to run. The following commands are available:
#### Primary commands
- `write` - write invoice to file
- `export` - export invoice to pdf
- `send` - send invoice via email
#### Chain commands
- `write-export` - write and export invoice in one command
- `write-export-send` - write, export and send invoice in one command

#### Options
- `-i` or `--id` - customer id
- `d` or `--date` - invoice date
- `H` or `--hour`  - number of hours
- `r` or `--rate` - hourly rate
- `--append` - append line to existing invoice

#### Misc
- `-h` or `--help` - show help message
- `--show` - show all profiles data
- `--open` [arg] - open configuration file in specified editor
  - `--open profile` - open profile configuration file
  - `--open credentials` - open credentials configuration file
  - `--open config` - open program config configuration file

## Examples
```shell
./invoice write -c 1 -d "23/09/2023"
```

## Configuration
The invoice CLI requires a configuration file to run. There are three configuration files:
- `config.json` - program configuration file:
  - This file contains core program configuration data, including program behaviour, file paths, and other miscellaneous data.
- `credentials.json` - email credentials configuration file
  - This file contains email credentials data, including email address and password. Note that you must use an app password if you are using Gmail. You can generate an app password [here](https://myaccount.google.com/apppasswords).
- `profile.json` - profile configuration file
  - This file contains profile data, including customer information and invoice information. You can add as many profiles as you want, but you have to comply with the following format:
```json
{
    "customers":[
        {
            "id": 1,
            "name": "customer name",
            "phone": "customer phone",
            "address": "customer address",
            "default_param": {
                "hours": 65.0,
                "rate": 477.0,
                "description": "description",
                "gst_code": "code"
            },
            "recipient_detail": {
                "recipient_email": "eamil@gmail.com",
                "subject" : "subject",
                "body" : "body..."
            }
        },
        ...
    ],
    "provider" : {
        "name": "provider name",
        "email": "email@gmail.com",
        "phone": "provider phone",
        "address": "provider address",
        "abn": "port abn",
        "payment_methods": "payment methods",
        "bsb": "provider bsb",
        "account_name": "provider account name",
        "account_number": "provider account number"
    }
}
```

> **Note:** The `default_param` field is optional. If you do not specify a default parameter, the program will use the default value in the program configuration file.

> **Note:** The `id` field must be unique. If you have multiple profiles, you must ensure that each profile has a unique id.


## Prerequisites (development)
#### OpenXLSX
Detail installation guide on openxlsx see [here](documentation\openxlsx_installation.md).

Also see README.md in [openxlsx github repo](https://github.com/troldal/OpenXLSX) for more information.

#### nlohmann-json
```shell
vcpkg install nlohmann-json
```
#### boost-program-options
```shell
vcpkg install boost-program-options
```
#### curl
```shell
vcpkg install curl
```