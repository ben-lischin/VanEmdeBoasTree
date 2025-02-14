# Van Emde Boas Tree

See the [report](report.pdf) for comprehensive design discussion, performance benchmarking against `std::set` BST, and theoretical guarantees.

### Running Locally
Install the necessary dependencies: `sudo apt install libssl-dev`

You are provided with the following make commands...

- `make test` - compile test.cpp for assessing VEB struct accuracy

- `make benchmark` (default) - compile benchmark.cpp for evaluating time/space performance for either data structure
- `make clean`

The ./benchmark executable has required item count param for generating the set, and optional "veb" or "bst" flags to specify a data structure for modeling the set. If no flag is specified, both will be created and benchmarked. This structure isolation flag is useful for measuring space usage as maximum resident set size during the process's lifetime.

For example, `/usr/bin/time -v ./benchmark 512000000 veb` will inform you of the time and space complexity of a Van Emde Boas Tree made representing 512 million uniform random items in the 32-bit world.