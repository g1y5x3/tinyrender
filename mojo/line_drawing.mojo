from renderer import TGAHeader, TGAColor, TGAImage

def main():
    image = TGAImage(100, 100, 3)
    image[10, 10] = TGAColor(255, 0, 0)
    image.write_tga_file("output.tga")
