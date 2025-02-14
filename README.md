# Van Emde Boas Tree

See the [report](report.pdf) for comprehensive design discussion, performance benchmarking against `std::set` BST, and theoretical guarantees.

### Running Locally
Install the necessary dependencies: `sudo apt install libssl-dev`

You are provided with the following make commands...

- `make test` - compile test.cpp for assessing VEB struct accuracy

- `make benchmark` (default) - compile benchmark.cpp for evaluating time/space performance for either data structure

- `make clean` - removes the executables

`./benchmark N` and `./test N` both require an input `N` defining the number of items that will be generated in the set. They will be selected uniformly at random from the universe.

`./benchmark` also has optional "veb" or "bst" arg to specify a data structure for modeling the set. By default, both will be created and benchmarked. This structure isolation flag is useful for measuring space usage as maximum resident set size during the process's lifetime.

For example, `/usr/bin/time -v ./benchmark 512000000 veb` will inform you of the time and space complexity of a Van Emde Boas Tree made representing 512 million items.