import argparse
from dataclasses import dataclass

@dataclass
class Vec3f:
    x: float = 0.0
    y: float = 0.0
    z: float = 0.0

class ObjModel:
    def __init__(self, filename: str):
        self.vertices: list[Vec3f] = []
        self.faces: list[tuple[int, int, int]] = []
        self.load_obj_file(filename)

    def load_obj_file(self, filename: str) -> bool:
        try:
            with open(filename, 'r') as file:
                lines = file.readlines()
        except IOError:
            print(f"Error: Could not open file {filename}")
            return False

        vertex_count = sum(1 for line in lines if line.startswith('v '))
        face_count = sum(1 for line in lines if line.startswith('f '))

        print(f"Number of vertices: {vertex_count}"
              f"\nNumber of faces: {face_count}")

        for line in lines:
            if line.startswith('v '):
                parts = line.split()
                vertex = Vec3f(float(parts[1]), float(parts[2]), float(parts[3]))
                self.vertices.append(vertex)
            elif line.startswith('f '):
                parts = line.split()
                face = (int(parts[1].split('/')[0]) - 1,
                        int(parts[2].split('/')[0]) - 1,
                        int(parts[3].split('/')[0]) - 1)
                self.faces.append(face)

        return True

if __name__ == "__main__":
    model = ObjModel("data/diablo3_pose.obj")