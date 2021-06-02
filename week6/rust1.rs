fn main() {
    let mut foo: u32;
  
    // Assigning a value to an uninitialized varaible is OK
    foo = 2;
    println!("foo = {}", foo);
  
    // Trying to again assign a value to an immutable variable is not allowed
    foo = 3;
    println!("foo = {}", foo);
  }