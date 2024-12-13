import os
import subprocess
from openai import OpenAI

# Model name
model_name = "Llama-3.2-11B-Vision-Instruct"
# Disable tokenizers parallelism warning
os.environ["TOKENIZERS_PARALLELISM"] = "false"

# Function to read content from a .txt file
def read_txt(file_path):
    try:
        with open(file_path, 'r', encoding='utf-8') as file:
            return file.read()  # Read the entire file content
    except Exception as e:
        print(f"Error reading file: {e}")
        return ""

# Function to write content to a .txt file
def write_txt(file_path, content):
    try:
        with open(file_path, 'w', encoding='utf-8') as file:
            file.write(content)
    except Exception as e:
        print(f"Error writing to file: {e}")

# Function to run make command and capture output
def run_make():
    try:
        result = subprocess.run(["make"], capture_output=True, text=True)
        return result.returncode, result.stdout, result.stderr
    except Exception as e:
        print(f"Error running make: {e}")
        return -1, "", str(e)

# Initialize OpenAI client
client = OpenAI(
    base_url="https://api.sambanova.ai/v1",
    api_key="yourapikey",
)

# Read the C++ code from the files
cpp_code = read_txt("/home/pl644/ece6775/finalproject/lab1/cordicbase.cpp")
header_code = read_txt("/home/pl644/ece6775/finalproject/lab1/cordic.h")

# Create the initial prompt with the code included
prompt = f"This is a C++ implementation of the CORDIC algorithm for calculating sine and cosine functions. Please complete the `cordic` function in the `cordic.cpp` file. Here is the code:\n\n{cpp_code}\n\nAnd here is the header file content:\n\n{header_code}"

# Initialize variables for tracking attempts
attempts = 1
max_attempts = 5  # Set a limit to avoid infinite loops

while attempts < max_attempts:
    # Send the API request
    completion = client.chat.completions.create(
        model=model_name,
        messages=[
            {
                "role": "user",
                "content": prompt
            }
        ]
    )

    # Get the response content
    response_content = completion.choices[0].message.content

    # Extract the code part from the response
    import re
    code_block = re.search(r'```(.*?)```', response_content, re.DOTALL)
    if code_block:
        code_content = code_block.group(1).strip()
    else:
        code_content = response_content.strip()

    # Remove any leading "cpp" from the code content
    if code_content.startswith("cpp"):
        code_content = code_content[3:].strip()

    # Write the code content to a file
    output_file_path = "/home/pl644/ece6775/finalproject/lab1/cordic.cpp"
    write_txt(output_file_path, code_content)
    print(f"Output written to {output_file_path}")

    # Run make command
    returncode, stdout, stderr = run_make()
    print(stdout)
    print(stderr)

    if returncode == 0:
        print("Compilation successful!")
        break
    else:
        print("Compilation failed. Updating prompt with error messages.")
        prompt += f"\n\nThe compilation failed with the following errors:\n{stderr}"
        attempts += 1

print(f"Compilation attempts: {attempts}")
