# cclunit-framework

The CCL Unit Framework is a collection of CCL programs for executing [CCL Unit Tests][ccl-unit-tests] and generating test results and code coverage data.  

It must be installed into a Cerner Millennium environment before it can be used. Find step-by-step instructions [here][step-by-step-installation-instructions].

## Current Version

3.4

## Usage

See [CCL Unit Tests][ccl-unit-tests] for the structure of a CCL Unit test case, instructions for executing one, and a rudimentary example.

See [CCL Unit Asserts][ccl-unit-asserts] for a list of all available asserts.

See [CCL Unit Mocking][ccl-unit-mocking] for details about the mocking API and a basic example using it.

See [CCL Unit Guidance][ccl-unit-guidance] for suggestions how to structure a program for testability and how to structure unit tests.
Short examples are provided to illustrate the techniques.

## Update Schedule

The update schedule is driven by [requests for][issues] and contributions of enhancements and corrections.  
See the [change log](CHANGELOG.md) for the contents of previous releases. Visit the [issues list][issues] to log a new request.

## Contribute

You are welcomed to contribute documentation improvements as well as code corrections and enhancements.  
Please read our [Contribution Guidelines][contibution_guidelines].

## Release

Committers:  
Please read and follow the [Release Guidelines][release_guidelines] when mergining a pull request.


## License

```
Copyright 2017 Cerner Innovation, Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
```

[ccl-maven-plugin]:https://github.com/cerner/ccl-testing/tree/master/ccl-maven-plugin
[cclunit-framework-installation]:./doc/FRAMEWORKINSTALL.md
[ccl-unit-tests]:./doc/CCLUNITTESTS.md
[contibution_guidelines]: CONTRIBUTING.md#contributing
[release_guidelines]: RELEASING.md
[mavenized CCL projects]: https://github.com/cerner/ccl-testing/tree/master/ccl-maven-plugin
[step-by-step-installation-instructions]: ./doc/FRAMEWORKINSTALL.md
[ccl-unit-mocking]: ./doc/CCLUTMOCKING.md
[ccl-unit-guidance]: ./doc/CCLUTGUIDANCE.md
[ccl-unit-asserts]: ./doc/CCLUTASSERTS.md
[issues]: https://github.com/cerner/cclunit-framework/issues