from dataclasses import dataclass

@dataclass
class TGAColor:
    r: int = 0
    g: int = 0
    b: int = 0
    a: int = 255

    def __post_init__(self):
        """ Validate color component ranges """
        for component in ('r', 'g', 'b', 'a'):
            value = getattr(self, component)
            if not (0 <= value <= 255):
                raise ValueError(f"Color {component}={value} out of range [0, 255]")
    
    def __getitem__(self, index: int) -> int:
        return (self.b, self.g, self.r, self.a)[index]

    @property
    def bgra(self) -> tuple[int, int, int, int]:
        return (self.b, self.g, self.r, self.a)

class TGAImage:
    def __init__(self, width, height, format='RGB'):
       self.width: int = width
       self.height: int = height
       self.bytes_per_pixel: int = {'GRAYSCALE': 1, 'RGB': 3, 'RGBA': 4}[format]
       self.data = bytearray(width * height * self.bytes_per_pixel)

    def set(self, x: int, y: int, color: TGAColor):
        index = (x + y * self.width) * self.bytes_per_pixel
        for i in range(self.bytes_per_pixel):
            self.data[index + i] = color[i]