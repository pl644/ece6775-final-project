# ece6775-final-project

The code includes the API implementation for LLM-generated Lab01 and Lab02. For Lab04, we utilized an LLM chatbot to assist with the implementation.

## Reproducing Results

To reproduce our results, follow these steps:

1. **Download All Files**: Clone the repository to your local machine.

    ```bash
    git clone https://github.com/pl644/ece6775-final-project.git
    ```

2. **Navigate to the Lab Directory**: Change directory to the specific lab folder (e.g., `lab1`, `lab2`).

    ```bash
    cd ece6775-final-project/finalproject/lab1
    ```

3. **Set Up Vivado HLS Environment**: Ensure you have Vivado HLS tool installed and properly configured on your system.

4. **Install Required Python Packages**: Install the necessary Python packages using pip.

    ```bash
    pip install google-generativeai
    pip install openai
    ```

5. **Run the Script**: Execute the script to generate and optimize the HLS code.

    ```bash
    python llama_version_anyB.py
    ```

Repeat these steps for each lab and model as needed. If you have any questions or run into issues, feel free to ask!
