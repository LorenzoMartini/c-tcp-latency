extern crate streaming_harness_hdrhist;

use std::io::BufReader;
use std::io::BufRead;
use std::fs::File;
use std::path::Path;
                
fn main() {   
    let f = match File::open("149.log") {
        Ok(file) => file,
        Err(e) => {
            panic!();
        }
    };

    let mut hist = streaming_harness_hdrhist::HDRHist::new();
    let file = BufReader::new(&f);
    for line in file.lines() {
        let res_line = line.unwrap();
        if res_line.starts_with("(") {
            let parsed_line = res_line.replace(")", "").replace("(", "").replace("\n", "");
            let mut line_iterator = parsed_line.split(",\t");
            let t_send = line_iterator.next().unwrap().parse::<u64>().unwrap();
            let t_recv = line_iterator.next().unwrap().parse::<u64>().unwrap();
            hist.add_value(t_send + t_recv);
        }
    }

    println!("-------------------------------------------------");
    println!("{:?}", hist.summary_string());
    println!("-------------------------------------------------");
    for line in hist.ccdf() {
        println!("{:?}", line);
    }
}
