""" Sigmoid LUT Generator

AUTHOR: Andrew Elliott
DATE: May 12th 2018

USAGE:
    This script generates a lookup-table in CSV form of the logistic sigmoid function.
    The output is formatted into fixed-point hexadecimal output.

    The Starting point (x) on the logistic input is always zero e.g. f(0) = 0.5
    and will produce entries for the table using the stop value (maximum x input)
    and the number of steps (table entries)

    Note that the output hex format is NOT Floating Point. It is a fixed point integer
    output of the form: U1.<precision>. There is one bit to signify when the output has reached
    the estimated saturation threshold for sigmoid, x > 8, f(x) = 1

"""

import numpy as np
import scipy
from scipy.special import expit
import os

# Generates a range from 0.0 to specified "stop" value
# 256 Entries for 8-bit LUT

filename = "sigmoid_lut_0_1.csv"    # Output File Name
stop = 1.0                          # End Value (of x range)
steps = 256                         # Number of table entries
precision = 15                      # Size of Fixed Point "Fractional Component" (unsigned)

# Output File Path and Name:
outfile = os.path.join(os.path.dirname(os.path.realpath(__file__)), filename)


# Convert to HEX
# Get maximum precision on fixed point output as an integer representation
# e.g. 0x5000 = 0.5
def fixed_point(x):
    fp = x
    tmp = x
    while True:
        if tmp > (2**precision)-1:
            return format(int(fp), '#x')
        fp = tmp
        tmp = tmp * 10


# Vars:
sigmoid_val = []
sigmoid_str = ""
cnt = 0


# Loop range and generate logistic outputs.
for x in np.arange(0.0, stop, (stop/steps)):
    res = expit(x)          # Logistic Function
    sigmoid_val.append(res)
    fp = fixed_point(res)   # Convert to hex fixed-point
    print("f(" + str(x) + ") = " + str(res) + " = " + str(fp))
    sigmoid_str += str(fp)
    cnt += 1
    if cnt < steps:
        sigmoid_str += "\n"

# Write result to file:
with open(outfile, "w") as out_file:
    out_file.write(sigmoid_str)

# Print Results:
print("\nEntries: " + str(cnt) + "\tStep Size: " + str(stop/steps) + "\tprecision: " + str(precision) + " bits")
print("\nThe file: " + outfile + " has been created.")
