from utils import StaticTuple

struct TGAHeader:
    var idlength: UInt8
    var colormaptype: UInt8
    var datatypecode: UInt8
    var colormaporigin: UInt16
    var colormaplength: UInt16
    var colormapdepth: UInt8
    var x_origin: UInt16
    var y_origin: UInt16
    var width: UInt16
    var height: UInt16
    var bitsperpixel: UInt8
    var imagedescriptor: UInt8

    fn __init__(out self):
        self.idlength = 0
        self.colormaptype = 0
        self.datatypecode = 0
        self.colormaporigin = 0
        self.colormaplength = 0
        self.colormapdepth = 0
        self.x_origin = 0
        self.y_origin = 0
        self.width = 0
        self.height = 0
        self.bitsperpixel = 0
        self.imagedescriptor = 0

    fn as_bytes(self):
        bytes = UnsafePointer[UInt8].alloc(18)
        bytes[0] = self.idlength
        bytes[1] = self.colormaptype
        bytes[2] = self.datatypecode
        bytes.free()

struct TGAColor(ImplicitlyCopyable, Movable):
    var bgra: StaticTuple[UInt8, 4] # BGRA order

    fn __init__(out self):
        self.bgra = StaticTuple[UInt8, 4](0, 0, 0, 255)

    fn __init__(out self, r: UInt8, g: UInt8, b: UInt8, a: UInt8 = 255):
        self.bgra = StaticTuple[UInt8, 4](b, g, r, a)

    fn __getitem__(self, index: Int) -> UInt8:
        return self.bgra[index]

    fn __setitem__(mut self, index: Int, value: UInt8) -> None:
        self.bgra[index] = value

struct TGAImage:
    var width: Int
    var height: Int
    var bytes_per_pixel: Int
    var data: List[UInt8]

    fn __init__(out self, width: Int, height: Int, bytes_per_pixel: Int):
        self.width = width
        self.height = height
        self.bytes_per_pixel = bytes_per_pixel
        self.data = List[UInt8](width * height * bytes_per_pixel, 0)

    fn __setitem__(mut self, x: Int, y: Int, color: TGAColor) -> None:
        index = (x + y * self.width) * self.bytes_per_pixel
        for i in range(self.bytes_per_pixel):
            self.data[index + i] = color[i]

    fn __getitem__(self, x: Int, y: Int) -> TGAColor:
        index = (x + y * self.width) * self.bytes_per_pixel
        color = TGAColor()
        for i in range(self.bytes_per_pixel):
            color[i] = self.data[index + i]
        return color

    fn write_tga_file(self, filename: String) -> None:
        try:
            with open(filename, "w") as f:
                header = TGAHeader()
                header.bitsperpixel = UInt8(self.bytes_per_pixel * 8)
                header.width = UInt16(self.width)
                header.height = UInt16(self.height)
                header.datatypecode = 2
                header.imagedescriptor = 0x00  # Bottom-left origin
                header.as_bytes()

                # f.write_bytes

        except IOError:
            print("Can't open file.")
        