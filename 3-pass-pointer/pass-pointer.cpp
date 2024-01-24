#include <systemc.h>
#include <iostream>
#include <vector>

// Step 1: Define the custom data type
struct kernel_info_t
{
    int id;
    std::string name;

    // Constructor for easy initialization
    kernel_info_t(int _id, std::string _name) : id(_id), name(std::move(_name)) {}
};

// Step 2: Create a vector of pointers and initialize them
SC_MODULE(MyModule)
{
    std::vector<kernel_info_t *> kernels;
    // Inside MyModule
    sc_signal<kernel_info_t *> my_signal;

    SC_CTOR(MyModule)
    {
        // Initializing the vector with distinct kernel_info_t objects
        kernels.push_back(new kernel_info_t(1, "Kernel1"));
        kernels.push_back(new kernel_info_t(2, "Kernel2"));
        kernels.push_back(new kernel_info_t(3, "Kernel3"));
        // ... more initializations as needed

        // Inside MyModule constructor
        SC_THREAD(change_signal);

        SC_METHOD(read_signal);
        sensitive << my_signal;
    }

    // Destructor to free memory
    ~MyModule()
    {
        for (auto &kernel : kernels)
        {
            delete kernel;
        }
    }
    // Other module contents will be added later

    // Inside MyModule
    void change_signal()
    {
        // This function changes the pointer stored in my_signal periodically
        // For demonstration, cycling through the vector
        static size_t index = 0;
        while (true)
        {
            wait(10, SC_NS);
            my_signal.write(kernels[index]);
            index = (index + 1) % kernels.size();
        }
    }

    void read_signal()
    {
        // This function reads the pointer from my_signal
        kernel_info_t *kernel = my_signal.read();
        if (kernel)
        {
            std::cout << "Kernel ID: " << kernel->id << ", Name: " << kernel->name << std::endl;
        }
    }
};

int sc_main(int argc, char* argv[]) {
    MyModule my_module("my_module");

    // Start the simulation
    sc_start(100, SC_NS); // Run for 100 nanoseconds

    return 0;
}
