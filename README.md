[![Build Status](https://travis-ci.org/bchurchill/pldi19-equivalence-checker.svg?branch=master)](https://travis-ci.org/bchurchill/pldi19-equivalence-checker) [![DOI](https://zenodo.org/badge/170968857.svg)](https://zenodo.org/badge/latestdoi/170968857)


# Semantic Alignment Equivalence Checker

This is an implementation of the equivalence checker presented in "Semantic
Program Alignment for Equivlance Checking" by Berkeley Churchill, Oded Padon,
Rahul Sharma and Alex Aiken, presented at PLDI 2019.  [Preprint PDF](https://raw.githubusercontent.com/bchurchill/pldi19-equivalence-checker/master/pldi2019.pdf).  This software is based on [STOKE](https://github.com/StanfordPL/stoke).

**Limitations.** This artifact can be used to reproduce many of the results of the
paper, but not all of them.  In particular, the paper describes a system to
discharge proof obligations concurrently using a large number of systems in the
cloud.  This artifact only supports discharging proof obligations on one core,
  and so it is much more limitted.  The artifact can be reliably use to check:
 - the strlen benchmark (section 5.3)
 - benchmark from [7] described in Section 5.4.
 - the running example (section 2)
 - _some_ of the TSVC benchmarks

There are a few reasons why the system described in the paper that uses cloud instances can verify more of the TSVC benchmarks than the artifact here:

 - The cloud system supports discharging proof obligations with multiple SMT solvers, e.g., both Z3 and CVC4, and choosing the fastest one.  The same goes for the choice of memory model.  Often a particular benchmark will have several difficult proof obligations, and different solvers are needed for each one.
 - This artifact does not implement a timeout for SMT solvers, so if a solver gets stuck, the whole problem gets stuck.
 - When multiple solvers and a timeout is available, a few of the benchmarks need a large amount of compute time -- e.g. 1000 CPU-hours.  The artifact runs each benchmark in a process that only utilizes 1 CPU core.

# Table of Contents

 1. [Getting Started](#getting-started)
    1. [Prerequisites](#prerequisites)
    1. [Using Docker](#using-docker)
    1. [Manual Setup](#manual-setup)
    1. [Running the Example](#running-the-example)
 2. [Step-by-Step Instructions](#step-by-step-instructions)
    1. [Running Example](#running-example-section-2)
    1. [Strlen Benchmark](#strlen-benchmark)
    1. [TSVC Benchmarks](#tsvc-benchmarks-sections-51-52)
    1. [APLAS17 Example](#example-from-dahiyas-2017-aplas-paper)
 3. [Running Your Own Benchmarks](#running-your-own-benchmarks)
    1. [Troubleshooting](#troubleshooting)
 4. [Understanding and Extending the Code](#understanding-and-extending-the-code)
    1. [Extending the Space of Invariants](#example-extend-the-space-of-invariants)
    1. [Supporting more x86-64 Instructions](#example-add-support-for-new-x86-64-instructions)
    1. [Extending the Space of Alignment Predicates](#example-extend-the-space-of-alignment-predicates)
 5. [Archived Execution Traces](#archived-execution-traces)
 6. [Post Publication Erata and Acknowledgements](#post-publication-erata-and-acknowledgements)

# Getting Started

## Prerequisites

You will need root access to a machine (physical or
virtual) with a Sandy Bridge processor or later. This is true of most
computers with Intel chips released 2012 or later. On linux, you can
check this by running `cat /proc/cpuinfo | grep avx`. If there are
results, then your machine is suitable. We require this because the
Sandy Bridge architecture adds the "Advanced Vector Extensions" (AVX)
instructions to x86-64, and our verification benchmarks may use these
instructions.  

You will need 20GB of disk space and at least 12GB of RAM (more is
better). Having extra cores and memory allows you to run multiple
benchmarks in parallel, but is not necessary.

## Using Docker

We have tested these instructions on linux with docker version
18.06.1-ce, but any operating system with a recent docker install
should be suitable.

1. Install Docker CE.  Follow the instructions for your platform:

- Windows https://hub.docker.com/editions/community/docker-ce-desktop-windows
- Mac https://hub.docker.com/editions/community/docker-ce-desktop-mac
- Ubuntu https://docs.docker.com/install/linux/docker-ce/ubuntu/
- Debian https://docs.docker.com/install/linux/docker-ce/debian/
- Fedora https://docs.docker.com/install/linux/docker-ce/fedora/
- CentOS https://docs.docker.com/install/linux/docker-ce/centos/

2. Test the docker install. Note that you may need to use `sudo` for
all docker commands:

```
$ sudo docker run hello-world
```

(should print a message containing "Hello from Docker!")

3. Pull the image from DockerHub

```
$ sudo docker pull bchurchill/pldi19
```

4. Run the image

```
$ sudo docker run -d -P --name eqchecker bchurchill/pldi19
```

5. Now you can SSH locally 

```
$ sudo docker port eqchecker 22
0.0.0.0:XXXXX

$ ssh -pXXXXX equivalence@127.0.0.1
(password is 'checker')
```

6. Build the code by running,

```
$ cd equivalence-checker
$ ./configure.sh
$ make
```

7. You may optionally run unit tests:

```
$ make test
```

8. Further a test script is available that ensures that our example benchmark works:

```
cd pldi19
./test.sh
```

9. When you're done with the artifact, you can cleanup by running

```
$ sudo docker stop eqchecker
$ sudo docker container rm eqchecker
$ sudo docker image rm bchurchill/pldi19
```

### Building Docker Images

You can also build your own docker images.  You will need to start by building the "base" image by running `sudo docker build -f Dockerfile.base .`, tag it, and then build the real image with `sudo docker build .`.  

## Manual Setup

We highly recommend using Docker because it ensures that all the right libraries and packages are installed.  A particular difficulty is getting the right version of `gcc` and related system libraries.  You should be able to get the software to run on newer distros, but the main trouble will be getting the compiler to work; `gcc-5` introduces some breaking changes.  Right now this tool works with `gcc-4.9`.

If you don't want to use Docker at all, there are instructions on building an environment suitable for compiling the code in `STOKE.md`.  In addition to those steps you will need to install `SageMath`.  You can also take a look at `Dockerfile` and `Dockerfile.base` to see how we build the environment.

## Running the Example

1. Be sure that you've compiled the code as described in the setup
instructions; run the tests for added assurances.

2. In `~/equivalence-checker/pldi19` you will find all the benchmarks
from the paper. For getting started, we will demonstrate running
our tool on the example from section 2 of the paper. Navigate to
`~/equivalence-checker/pldi19/paper_example`.

3. To see the source code for the programs we are comparing, run `cat source.c`. 
We will prove that `bitflip()` performs the same computation as `bitflip_vec()`.

4. Run `make` to compile the functions with `gcc -O1` and disassemble
them. The assembly code is written into the folder `opt1`.

5. Our tool needs test cases to find an alignment between the two
programs. For this example, we use a symbolic execution tool to
generate test cases for execution paths up to a bound. Run 
`make tcgen`. This should finish within 10 seconds. The generated testcases
can be found in the `testcases` file; each testcase includes a value
for each machine register and values for a subset of memory locations.

6. Run `./demo.sh | tee trace` to perform verification and save the
output to the file called `trace`. If verification succeeds, you will
see the message `Equivalent: yes` at the end of the output. This
should finish in under 30 seconds; it may appear to hang briefly while
some matrix computations are performed.

7. Review the trace file.  It contains:

 - The assembly of the two programs being compared.
 - The alignment predicates guessed, in order. e.g. "Trying alignment
   predicate..."
 - For some alignment predicates, the PAA that was initially
   constructed before simplification.
 - The PAA after simplication.
 - If the PAA accepts all test cases, there will be output from
   learning the invariants.  You should see matrices here.
 - After invariant learning, you will see the discharge of a series of
   proof obligations.
 - For our examples, once proof obligations start being checked
   the proof usually succeeds (but this isn't guaranteed -- another
   alignment predicate may be tried if they fail).
 - At the end of the trace, the final PAA along with all the
   invariants are printed.

Note that the trace file may contain some ANSI-escape codes like
`<E2><89><A4>`. These can be used to render colors in the output for
debugging counterexamples from the SMT solver. If you're not trying to
debug, these can be ignored.

To gain a deeper understanding of the PAA, one needs to see how the
basic blocks of the two programs are numbered.

# Step-by-Step Instructions

## Running example (Section 2)  
(see also [Getting Started](#Running-the-example))

 1. Navigate to `~/equivalence-checker/pldi19/paper_example`
 2. Run `make`
 3. Run `make tcgen`
 4. Run `./demo.sh`

## Strlen Benchmark

  1. Navigate to `~/equivalence-checker/pldi19/strlen`.

  2. Run `make`

  3. Run `./demo.sh`. Depending on your machine, it should finish 
  within 10 minutes and end with "Equivalent: yes".

Getting the right test cases for strlen is tricky; in the image we
have included a set that will certainly work. We've also provided
a script to generate them, but sometimes these test cases don't
provide sufficient code coverage. You can try regenerating the test
cases using `make tcgen` and updating `demo.sh` to point to the
`testcases` file rather than `testcases.good`. It may take a few
tries.

## TSVC benchmarks (Sections 5.1, 5.2)

 1. Navigate to `~/equivalence-checker/pldi19/TSVC`
 2. Run `make`. This will build the benchmarks in clean.c with each of
    gcc -O1 ("baseline"), gcc -O3 ("gcc") and clang -O3 ("llvm").
 3. Run `make tcgen`. This randomly generates a new set of test
    cases for the TSVC benchmarks using a utility we have specifically
    constructed for this purpose.  All benchmarks use the same set
    of test cases, except s176 uses fewer.
 4. Run `cat benchmarks` to see the list of available benchmarks.
 5. To run a benchmark, use 

```
$ ./demo.rb verify <compiler1> <compiler2> <benchmark-name>
```

for example:

```
$ ./demo.rb verify baseline gcc s000
```

 The output will be written to a file in the `traces` folder, any
 errors to a file in the `misc` folder, and a report of the time
 into the `times` folder. The exact filenames can be seen in the
 output of the demo.rb script.

 In the paper, we have performed the baseline-gcc comparison and
 the baseline-llvm comparison for each benchmark.

 The benchmarks below should be able to run reliably without
 the cloud infrastructure.  Others may work too, depending on
 your hardware (and patience!).  By default these run using Z3
 and the flat memory model.  One can also try CVC4 by editing
 `demo.rb`

 - s000-gcc
 - s000-llvm
 - s1112-gcc
 - s121-gcc
 - s121-llvm
 - s1221-gcc
 - s1221-llvm
 - s1251-gcc
 - s1351-gcc
 - s1351-llvm
 - s173-gcc
 - s2244-gcc
 - vpv-gcc
 - vpvpv-gcc
 - vpvtv-gcc
 - vtv-gcc
 - vtvtv-gcc
 

 6. If you have several cores and extra memory available, you can use
the following commands to run multiple benchmarks in parallel:

```
./demo.rb verify-all <filename>
./demo.rb verify-gcc <filename>
./demo.rb verify-llvm <filename>
```

In each case, filename contains a newline-delimited list of
benchmarks (e.g. like the 'benchmarks' file). In general it's best
to have one core per benchmark running concurrently. verify-all
invokes the validator twice for each benchmark, once for gcc and
once for llvm, while verify-gcc and verify-llvm just compare the
selected compiler with the baseline.

The files `benchmarks.1` and `benchmarks.2` each list half of
the benchmarks functions (14 each). So, if you have 14+ cores and a lot of
RAM (e.g. 256GB), you can try running './demo.rb verify-gcc benchmarks.1' to
run 14 of the benchmarks in parallel. However, we haven't tested
all of the benchmarks in single-threaded execution, and some could
take a very long time to finish.

## Example from Dahiya's 2017 APLAS paper.

1. Navigate to `~/equivalence-checker/pldi19/aplas17`

2. Run `make`

3. Run `./demo.sh`.

For this benchmark, a working set of test cases is provided in the repository
(and the Docker image).  However, one can also copy a randomly generated
testcase file from the TSVC benchmarks into this folder and use those instead.

# Running Your Own Benchmarks

The easiest way to run your own benchmark is to copy the
`paper_example` folder, run `make clean`, and then update the C code
for your benchmark. There are a few other things that need to be
updated as well:

1. If you have re-named the functions, update the TARGET and REWRITE
in the `variables` file with the paths to the generated assembly code.

2. If you have changed the parameter types or the return types
of the function, you will need to update the DEF_INS and LIVE_OUTS
sets in the `variables` file. Each of these variables contains a
set of x86-64 registers. The best way to find the correct setting
is to lookup the x86-64 System V ABI's calling convention (which
places integer/pointer parameters in rdi, rsi, rdx, rcx, r8, r9, and
integer/pointers return values into rax) or to read the assembly code.

3. The biggest and most important thing to update are the test
cases. The simplest thing to try is to just run `make tcgen`. This
will try to use a symbolic executor to make test cases. However,
there are two important caveats. First, the symbolic executor can't
guarantee code coverage. If verification fails, you should try giving
the system more test cases (although this slows it down). You can
(a) increase the number of test cases used for training by supplying
the --training_set_size parameter (defaults to 20); and/or (b) set a
higher bound in the Makefile for the symbolic executor.

Second, if your binary contains read-only data (e.g. addressed via
RIP-offset addressing) you need to do extra work. You need to manually
create a file called `rodata` with a test case that contains all the
read-only memory locations along with values. Then, ./demo.rb needs
to be updated with a flag `--rodata </path/to/rodata>`. An example
of this file can be found in the `~/equivalence-checker/pldi19/TSVC`
folder. We unfortunately don't have tools to automate this (yet), so
it's easier to stick with functions that don't require read-only data.

It is important to double-check that your test cases actually work
for both programs. Problems can come up, for example, when one
program reads memory locations that the other program doesn't (as in
the strlen benchmark, see section 5.3, esp. lines 1000-1009). The
tool will give warnings that appear after "COLLECTING DATA..." if
the test cases don't work right. The command `stoke debug sandbox
--target </path/to/assembly.s> --testcases </path/to/testcases>
--index <N> --debug` can be used to understand why a particular test
case is failing. 

For long running loops, you may need to provide the `--max_jumps`
option to the verification tool, which is used to stop and abort
any loops that seem to be executing forever when performing dynamic
analysis (the default value is 1024 iterations). However, this is
rarely the bottleneck for our benchmarks since the symbolic executor
doesn't generate test cases that run for so many iterations.

4. We have placed bounds on the values of `rsi` and `rdi` to avoid
certain overflow conditions. If you need this for your benchmark, you
can add them with the `--assume` option in `demo.sh`. The `--assume`
option parses a limited set of expressions where the leaf nodes are
of the form `X_%reg`, where `X` is either `t` or `r` (for `target` or
`rewrite`) and `%reg` is an x86-64 register name. The parser and its
grammar can be found in `~/equivalence-checker/src/expr/expr_parser.h`

(v) It's unlikely you will need to change it, but you can try
increasing `TARGET_BOUND` and `REWRITE_BOUND` to 30 in the `variables`
file as a fail-safe option.

## Troubleshooting

If a benchmark is failing (i.e. execution isn't ending or it returns
"Equivalent: no") there are a few possible causes:

  1.  The two programs might not be equivalent.
  2.  The correct alignment predicate is not in the search space.
  3.  A good alignment predicate is found, but the learned invariants
      aren't strong enough for a proof.
  4.  There aren't enough test cases to create a correct PAA or learn correct
  invariants.  That is, the test cases might not cover all program behaviors.
  5.  The proof obligations are taking a long time -- so long that the search
  space isn't being explored quickly enough.
  6.  There's a bug in the tool.

Some things to try:

  1. Use the bounded validator to see if actually the programs are not
equivalent. The bounded validator is unsound, but for terminating programs it
is complete with a sufficiently high bound. Many of the examples come with a
`./bounded.sh` in their folder for this purpose.  The `stoke_debug_verify`
tool can be told to use the bounded validator by provided `--strategy
bounded`, along with a `--bound <N>` parameter.  Alternatively, you can test
the code or check the correctness by hand.

  2. Try supplying your own alignment predicate. Doing so eliminates
failure mode #2 above and also more clearly isolates the problem
in all the other cases (it sometimes resolves failure modes #5 and #6). This can be done by supplying the `--alignment_predicate`
argument along with an expression. It parses a limited set
of expressions where the leaf nodes are of the form `X_%reg`,
where X is either `t` or `r` (for 'target' or 'rewrite') and
`%reg` is an x86-64 register name. The parser and its grammar can
be found in `~/equivalence-checker/src/expr/expr_parser.h`. For
example, you can specify `--alignment_predicate "t_%rax+1=%r_rdx"`
to align traces when the target program's value for `%rax` is
one less than the rewrite's value for `%rdx`. You can also use
`--alignment_predicate_heap` to force the traces to only align when
heap states match.

  3. If you have supplied your own alignment predicate but there's
still a failure, then there are two possibilities:

(A) The PAA is constructed successfully and it accepts all the test
cases. The proof obligations start to discharge, but ultimately it
fails. This suggests failure mode #3 or #4. One needs to inspect
the failed proof obligation and SMT counterexample to determine
if it's because the PAA is missing an edge (#4) or if the tool is
failing to prove a necessary invariant (#3). The tool is generally
quite verbose, and in addition to giving a counterexample for failed
proof obligations it dynamically executes the counterexample as an
error-detecting strategy (to defend against failure mode #6) and
reports if the dynamic execution differs from an expected value.

(B) The PAA can't be built or it doesn't accept all the test cases.
Here, the most likely causes are that the alignment predicate is
still wrong (#2), you need more test cases (#4), or the programs
aren't actually equivalent (#1).

# Understanding and Extending the Code

This section is intended as a brief description of the code base for
anyone who wants to extend the code. The `~/equivalence-checker/src` and
`~/equivalence-checker/tools` folders contain all the source code for
our tools. The tools folder has the command line tools which make use
of the code in the src folder to do all the heavy lifting.

If ever you want to rebuild the code, running `make` in the top level
directory should be sufficient. A `make clean` usually helps if there
seems to be a problem.

The src folder has a number of subfolders:

  `validator` - This is where the equivalence checker resides, along
with semantics for all the x86-64 instructions, all the code for
alignment, building the PAA, discharging proof obligations, etc.

  `symstate` - Data structures for representing an x86-64 system
symbolically, along with abstractions of symbolic bitvectors and
arrays that serve as a frontend to the SMT solver.

  `solver` - Interfaces with SMT solvers.

  `state` - Data structure for representing the concrete state of an
x86-64 system.

  `sandbox` - Code to execute safely x86-64 code concretely against our
internal state representation.

  `cfg`, `tunit` - Data structures for representing assembly code as
control flow graphs and static analysis.

  `expr` - An expression parser

  `diassembler` - The disassembler

  `ext` - folder for external code. Most notably includes Z3, CVC4 and
x64asm (the library used to JIT assembly code and run it for the
sandbox).

  `serialize, stategen, kerberos, target, unionfind` - Other utilities (not relevant)

  `cost, search, transform, verifier` - Used by the superoptimizer (not relevant)

Within the validator folder:

  `ddec.cc` - This is where our algorithm is implemented.  It all begins
    in the verify() function.  (The name DDEC is in reference to
    "Data-Driven Equivalence Checking" by Sharma et al, which our
    work extends)

  `handlers` - semantics for x86-64 instructions

  `invariants` - data structures to represent different kinds of invariants

  `variable.cc` - An abstraction to represent registers, memory locations, 

  `paa.cc` - The representation of the program alignment automata, and some
of the important methods.  `learn_state_data` is where the PAA checks that
it accepts the test inputs and gathers states to learn invariants.

  `learner.cc` - Code to learn invariants from concerete executions.

  `data_collector.cc` - An abstraction on top of the sandbox to collect
data from concrete execution traces

  `smt_obligation_checker.cc` - Code to check the proof obligations.

  `int_matrix.cc` - Uses sage to compute nullspaces over an integer ring.

  `sage.cc` - Interface with SageMath

Since the tool is built upon STOKE, consulting the STOKE
documentation may be helpful too. This can be found at
https://github.com/StanfordPL/stoke

## Example: Extend the space of invariants.

If you want to add an invariant to the system, there are two things
you need to do:

1. Add the invariant in the `invariants` folder. This means writing
code to evaluate whether the invariant holds over a pair of symbolic
states and over a pair of concrete states.

2. Adjust `learner.cc` to learn this invariant from a concrete execution.

## Example: Add support for new x86-64 instructions.

The key step is to add a new "handler" in the "handlers" folder. This
is discussed in more detail in a `README.md` file which appears in the
`src/validator` folder. (That documentation is a little dataed, but
correct enough to make progress).

## Example: Extend the space of alignment predicates.

Here, you will want to change the `verify()` function in `ddec.cc` to
construct a different set of alignment predicates to pass to the
`test_alignment_predicate` function.

# Archived Execution Traces

Since some of the TSVC benchmarks take a long time to run, we
have included in the artifact traces from these benchmarks from
successful runs using the cloud instances. These can be found in the 
`pldi19-traces.tar.xz` file. These traces are from the tool around the
time of submission -- the exact text in the traces differs from what 
the tool currently produces.

# Post Publication Erata and Acknowledgements

Special thanks to the following who have helped find and fix bugs in our software:

 * Nikhil Durgam of IIT Bombay discovered a soundness bug when using the flat memory model.  Our tool would generate incorrect constraints to check that the final memory states were equal. (Mar 31, 2020) 

