import httplib
import sys

http_server = "localhost:8000"
connection = httplib.HTTPConnection(http_server)

# Open test input. 
test_file_path = "test_input"
test_f = open(test_file_path)
inputs = test_f.readlines()
inputs = [x.strip() for x in inputs]
test_f.close()

# Open expected input.
expected_file_path = "expected"
expected_f = open(expected_file_path)
expecteds = expected_f.readlines()
expecteds = [x.strip() for x in expecteds]
expected_f.close()
assert(len(inputs) == len(expecteds))	

for i in range(len(inputs)):
  connection.request("GET", ("<start>%s<end>" % inputs[i]))
  response = connection.getresponse()
  if response.status != 200:
    print("Request failed for input: %s. Reason: %s" % (inputs[i], response.reason))
  output = response.read()
  print("Output:", output)
  print("Expected:", expecteds[i])
  if expecteds[i] == output:
    print("SUCCESS")
  else:
    print("FAILURE")

