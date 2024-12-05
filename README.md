# Network Reconstruction and Optimization
### Steps to Run

1. **Open the Terminal**:
   - Launch your terminal application.

2. **Navigate to the Project Directory**:
   - Use the `cd` command to go to the directory where the project is located.
     
3. **Compile the Project:**
   -In the terminal, type the following command to build the project:
   ```bash
   make all
   
4. **Run the Program:**
   - After compiling, run the program with the required arguments. Use the following command structure:

   ```bash
   ./reconstitueres 05000_USA-road-d-NY.cha <method>
   
   Where:
   - 05000_USA-road-d-NY.cha is the .cha file you want to process (you can replace it with any other .cha file).
   - <method> is one of the following:
      -0 for list
      -1 for hashmap
      -2 for quadtree
   
   Example:
   ```bash
   ./reconstitueres 05000_USA-road-d-NY.cha 1

