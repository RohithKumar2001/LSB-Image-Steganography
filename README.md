# LSB-Image-Steganography
Steganography is the practice of concealing secret information within a carrier medium to avoid detection. It follow the principle that a hidden message can be effectively concealed within an innocuous cover object, such as an image, audio file, video, or text, without evoking suspicion.
There are several methods of steganography, including:
1. LSB (Least Significant Bit) Embedding
2. Noise-based Steganography
3. Whitespace Steganography
4. Digital Watermarking

LSB (Least Significant Bit) Image Steganography is a technique used to hide information within the least significant bits of digital images. In digital images, each pixel is represented by binary values corresponding to its color components (e.g., red, green, and blue in RGB images). The least significant bit of each color component is the bit that has the smallest effect on the pixel's color. By replacing these least significant bits with data from a secret message, it's possible to embed the message into the image without significantly altering its appearance.

Requirements:
• Choose an image as the carrier for the hidden message. The carrier image should be a common image format, such as BMP.
• Create the secret message that you want to hide within the image. This message can be text.
• Analize the size of the message file and the data part of the .bmp file to check whether the messsage could fit in the provided .bmp image
• Encode the least significant bits of the carrier image's color components with bits from the secret message.
• The image resulting from the encoding process contains the hidden message. Visually, the image will appear similar to the original carrier image, but the hidden message is embedded within its LSBs.
• To extract the hidden message from the image, reverse the process by extracting the least significant bits from each pixel and converting them back into the original message format.
• Provide a option to steg a magic string which could be usefull to identify whether the image is steged or not 
• The application should provide a option to decrpt the file 
• This has to be an command line application and all the options has to be passed as an command line argument
