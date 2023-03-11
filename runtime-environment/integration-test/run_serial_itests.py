import subprocess, sys
if len(sys.argv) < 2:
    print("Please give test count as an argument")
    exit()

test_count = sys.argv[1]

if not test_count.isdigit():
    print("Please provide an integer as an argument")
    exit()

# Define the command to be executed
cmd = ["python3", "run_itests.py"]

test_count = int(test_count)

for i in range(0, test_count):
    # Use subprocess to start the process in the background
    process = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    # Wait for the process to complete
    process.wait()

    # Check the return code of the process
    if process.returncode == 0:
        print("Serial test {} completed successfully.".format(i+1))
    else:
        print("Process failed with return code:", process.returncode)
