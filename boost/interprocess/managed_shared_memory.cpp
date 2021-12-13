#include <boost/interprocess/managed_shared_memory.hpp>
#include <iostream>
#include <ostream>
#include <cstdlib> //std::system

// example description:
// - parent process creates a named managed shared memory segment, constructs a string into it and invokes a child process
// - child process opens the same memory segment, reads and prints the string content

int main(int argc, char *argv[])
{
    namespace boost_ipc = boost::interprocess;

    std::string hello_shm = "hello_shm";
    
    if (argc < 2) // parent process
    {
        struct shm_remove{
            shm_remove() { boost_ipc::shared_memory_object::remove("My_SHM"); }
            ~shm_remove(){ boost_ipc::shared_memory_object::remove("My_SHM"); }
        } remover;
        
        std::cout << "first app!" << std::endl;

        boost_ipc::managed_shared_memory segment(boost_ipc::create_only, "My_SHM", 1024);
        std::string * hello_instance = segment.construct<std::string>
            ("my_string_instance")
            (hello_shm);
        
        std::string s(argv[0]);
        s += " child ";
        // invoke child process
        if(0 != std::system(s.c_str())){
            return 1;
        }
    }
    else // child process
    {
        std::cout << "second app!" << std::endl;

        boost_ipc::managed_shared_memory segment(boost_ipc::open_only, "My_SHM");
        std::pair<std::string *, boost_ipc::managed_shared_memory::size_type> result;
        result = segment.find<std::string> ("my_string_instance");

        std::cout << "segment string = " << *result.first << std::endl;
    }
    
    std::cout << "bye now!"<< std::endl;
    return 0;
}
