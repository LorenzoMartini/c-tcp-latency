# c-tcp-latency
Program to measure TCP RTT latency between a server and a client.
The program will measure cycles it took to perform a send and how many cycles it took from after sending to complete receive.
If you want to measure time instead of cycles, just `lscpu` the frequency of your machines and derive the time from the cycles calculated.
Otherwise, you could make use of one of my other tools, namely [rust-tcp-io-perf](https://github.com/LorenzoMartini/rust-tcp-io-perf) that measures either latency or bandwidth of a connection.

## Usage
You will need to run the server and the client separately, on the two machines you want to benchmark the latency between.
Open a terminal on both machines (either directly or via SSH) and clone the repo, `cd` into the `src` folder and run `make` to compile.

Once compiled, first start up the server machine, by staying in the `src` folder and running the command
```
./server -p <port> -n <n_bytes>
```
where `<port>` is the port you want to listen on and `<n_bytes>` the size of the messages you will receive.

Similarly, to run the client, from the `src` folder, run
```
./client -a <server_address> -p <port> -n <n_bytes>
```
Where `<server_address>` is the addres (or hostname) of the server machine, `<port>` the same port you ran server with and `<n_bytes>` the same used for server (size of the messages you will send).

The client program, once done, will printo out results for 1M iterations of latency measure, in the format `(<send>, <receive>)`, where `<send>` is the time it took to perform the send, and `<receive>` the time it took from after the send to right after the receive is completed.
