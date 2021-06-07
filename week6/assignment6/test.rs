// Mateo Estrada
// Oregon State University
// Operating Systems CS344
// Assignment 6

use std:: env; // to get arguments passed to the program
use std::thread;
/*
* Print the number of partitions and the size of each partition
* @param vs A vector of vectors
*/

fn print_partition_info(vs: &Vec<Vec<usize>>){
    println!("Number of partitions = {}", vs.len());
    for i in 0..vs.len(){
        println!("\tsize of partiton {} = {}", i, vs[i].len());
    }
}


/*
* Create a vector with integers frm 0 to num_elements - 1
* @param num_elements How many integers to generate
* @ return a Vector with integers from 0 to (num_elements - 1)
*/

fn generate_data(num_elements: usize) -> Vec<usize>{
    let mut v : Vec<usize> = Vec::new();
    for i in 0..num_elements{
        v.push(i);
    }
    return v;
}

/*
* Partition the data in the vector v into 2 vectors
* @param v Vector of integers
* @return A vector that contains 2 vectors of integers
*/
fn partition_data_in_two(v: &Vec<usize>) -> Vec<Vec<usize>>{
    let partition_size = v.len() / 2;
    // create a vector that will contain vectors of integers
    let mut xs: Vec<Vec<usize>> = Vec::new();

    // Create the first vector of integers
    let mut x1 : Vec<usize> = Vec::new();

    // add the firs half of the integers in the input vector to x1
    for i in 0..partition_size{
        x1.push(v[i]);
    }
    // Add x1 to the vector that will be returned by this function
    xs.push(x1);

    // Create the second vector of integers
    let mut x2 : Vec<usize> = Vec::new();
    // Add the second half of the integers in the input vector to x2
    for i in partition_size..v.len(){
        x2.push(v[i]);
    }
    // Add x2 to the vector that will be returned by this function 
    xs.push(x2);
    // return the result vector
    xs
}


/*
* Sum up all the integers in the given vector
* @param v Vector of integers
* @return Sum of integers
* Note: this function has the same code as the reduce data function.
*       But don't change the code of mat_data or reduce_data
*/
fn map_data(v: &Vec<usize>) -> usize{
    let mut sum = 0;
    for i in v{
        sum+=i;
    }
    sum
}

/*
* Sum up all the integers in the given vector
* @param v Vector of integers
* @ return Sum of integers in c
*/

fn reduce_data(v: &Vec<usize>) -> usize{
    let mut sum = 0;
    for i in v{
        sum+=i;
    }
    sum
}

/*
* A single threaded map-reduce program
*/

fn main(){
    // use std::env to get arguments passed to the program
    let args: Vec<String> = env::args().collect();
    if args.len() != 3{
        println!("ERROR: usage {} num_partitions num_elements", args[0]);
        return;
    }

    let num_partitions : usize = args[1].parse().unwrap();
    let num_elements : usize = args[2].parse().unwrap();

    if num_partitions < 1{
        println!("ERROR: num_partitions must be at least 1");
        return;
    }
    if num_elements < num_partitions{
        println!("ERROR: num_elements cannot be smaller than num_partitions");
        return;
    }

    // Generate the data:
    let v = generate_data(num_elements);

    // PARTITION STEP: partition the data into 2 partitions
    let xs = partition_data_in_two(&v);

    // print info about the partitions
    print_partition_info(&xs);

    let mut intermediate_sums : Vec<usize> = Vec::new();

    // MAP STEP: Process each partition
    // CHANGE CODE STARTS HERE: dont change anything above this.
    // Change the following code to create 2 threads that run concurrently 
    // and each of which uses map_data() function to process one of the two partitions
    // Following this example: https://replit.com/@cs344/104threads
    // taking into account that I have to pass the reference of xs so I have to move
    // I must declare the type of the result vector rv thread::JoinHandle
    // Clone the partition vector to use it 
    let mut rv: Vec<thread::JoinHandle<_>> = Vec::new();
    let xn = partition_data(num_partitions, &v);
    let mut intermediate_sums_n : Vec<usize> = Vec::new();
    // iterate to the number of partitions to make the threads and place them
    // on a vector that was built specifically for this JoinHandle type
    
    for i in 0..num_partitions{
        let val = xn[i].clone(); // clone the content of the ith value
        rv.push(thread::spawn(move || map_data(&val))); // push them onto rv

    }
    // Time to iterate again to the num of partitions and then
    // join().unwrap() the threads
    for _j in 0..num_partitions{
        let tn = rv.remove(0); // remove the first one
        let sum = tn.join().unwrap();
        intermediate_sums.push(sum); // save the sums on the intermediate_sums vector
    }
    // Clone the output from the two partitions
    let xs0 = xs.clone();
    // loop to make two threads to run concurrently
    let t0 = thread::spawn(move || map_data(&xs[0]));
    let t1 = thread::spawn(move || map_data(&xs0[1]));
    let _j1 = t1.join().unwrap(); // unwrap them
    let _j0 = t0.join().unwrap();
    intermediate_sums_n.push(_j1); // put in intermediate sums vector
    intermediate_sums_n.push(_j0);
    // print the vector with the intermediate sums
    println!("Intermediate sums = {:?}", intermediate_sums_n);
    //REDUCE STEP: Process the intermediate result to produce the final result
    let sum_n = reduce_data(&intermediate_sums_n);
    // print the sum
    println!("Sum = {}", sum_n);
    // print the info from the n threads
    print_partition_info(&xn);
    // CHANGE CODE END: Don't change any code below this
    // print the vector with the intermediate sums
    println!("Intermediate sums = {:?}", intermediate_sums);
    //REDUCE STEP: Process the intermediate result to produce the final result
    let sum = reduce_data(&intermediate_sums);
    println!("Sum = {}", sum);


}

fn partition_data(num_partitions: usize, v:&Vec<usize>) -> Vec<Vec<usize>>{
    // partition data in equal sized partitions based on the argument num_partitions
    let mut xs: Vec<Vec<usize>> = Vec::new();
    let partition_size = v.len() / num_partitions;
    // If the number of values isn't divisible by the number of partitions then
    // a remainder amount of partitions will have one more
    let mut remainder = v.len() % num_partitions;

    // The loop is designed to place the remainders into partitions when the number of elements
    // is not a multiple of the number of partitions.
    let mut start_int = 0;
    let mut end_int = partition_size;
    let mut count_nums = 0;

    // cycling through partitions
    while count_nums < num_partitions{
        // Make a vector x of usize
        let mut x : Vec<usize> = Vec::new();
        println!("{}, {}", remainder, partition_size);

        for i in start_int..end_int{
            x.push(v[i]);
        }
        xs.push(x);
        // If modulus == 0, it means that the partition size is even, the start and end can change
        if remainder == 0{
            // adding the start int and end int by the partition size
            
            start_int += partition_size;
            end_int += partition_size;
            println!("{}, {}", start_int, end_int);
        }
        if remainder > 0{
            // I will add 1 value extra to the vector, and keep doing that while decreasing 
            // mod until mod is 0
            xs[count_nums].push(v[end_int]);
            start_int += partition_size + 1; // add that extra value
            end_int += partition_size + 1; // add that extra value
            remainder -= 1; // remove one from the remainder and add it to the xs
        // if remainder is zero then change the start number and the end number to where we are now
        }
        count_nums+= 1;
    }
     // return the final vector 
     xs
}


