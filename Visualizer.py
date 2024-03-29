"""
Assumes all matrices are the same size.

"""
# Note: PIL needs to have the memory in the numpy array be exact. So uint8 type is used to make the bytes line up
import numpy as np
import os
from PIL import Image

BASENAME = "final"
NUM_FRAMES = 10

INPUT_DIR = "./out/"
OUTPUT_DIR = "./frames/"
SCALE_FACTOR = 15
Matrix = np.ndarray


def load_matrix(file) -> Matrix:
    line = file.readline()
    m, n = line.strip().split(" ")
    m = int(m)
    n = int(n)
    
    mat = []
    for line in file:
        curr = []
        for val in line.split():
            curr.extend([np.uint8((1 - int(val)) * 255) for _ in range(SCALE_FACTOR)])
        # print(curr)
        mat.extend([curr for _ in range(SCALE_FACTOR)])

    # mat = [[int(val) for val in line.split()] for line in file]
    return np.array(mat)


def matrix_to_frame(matrix: Matrix) -> Image:
    return Image.fromarray(matrix, mode="L")


gif: list[Image] = []

for i in range(NUM_FRAMES):
    with open(f"{INPUT_DIR}{BASENAME}{i}.matrix", "r") as fp:
        matrix = load_matrix(fp)
        gif.append(matrix_to_frame(matrix))

# arr = np.asarray(gif[0])
# print(arr)
# gif[1].show()
gif[0].save("Result.gif", save_all=True, append_images = gif[1:], loop=0, duration=500)

# print(os.listdir("./out/"))
    
    