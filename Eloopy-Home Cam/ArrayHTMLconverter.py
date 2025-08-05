import re
import gzip
import binascii

#Put camera_index.h in the same folder.
#Extract and save the original HTML as index_ov2640.html.
#Wait for you to edit and save a new file as modified_index_ov2640.html.
#Re-encode it to index_ov2640_array.h in the same C array format.

# Input and output paths
header_file = 'camera_index.h'
html_output = 'index_ov2640.html'
modified_html_input = 'modified_index_ov2640.html'
c_array_output = 'index_ov2640_array.h'

# Step 1: Extract the first C array block from the .h file
def extract_first_block(file_path):
    with open(file_path, 'r') as f:
        content = f.read()

    match = re.search(r'const unsigned char.*?=\s*\{(.*?)\};', content, re.DOTALL)
    if not match:
        raise ValueError("No C array block found.")

    hex_values = re.findall(r'0x[0-9a-fA-F]{2}', match.group(1))
    print(f"[Extract] Extracted {len(hex_values)} bytes from the first C array.")
    return bytes(int(h, 16) for h in hex_values)

# Step 2: Decompress and save HTML
def decompress_and_save_html(data, output_path):
    html = gzip.decompress(data)
    with open(output_path, 'wb') as f:
        f.write(html)
    print(f"[Save] Decompressed HTML saved to: {output_path}")

# Step 3: Recompress modified HTML and convert to C array
def reencode_html_to_c_array(input_path, output_path, array_name):
    with open(input_path, 'rb') as f:
        html = f.read()
    compressed = gzip.compress(html)

    # Format as C array
    lines = []
    for i in range(0, len(compressed), 20):
        chunk = compressed[i:i+20]
        line = ', '.join(f'0x{b:02X}' for b in chunk)
        lines.append("  " + line)
    c_array = f"""#define {array_name}_len {len(compressed)}
const unsigned char {array_name}[] = {{
{',\n'.join(lines)}
}};
"""
    with open(output_path, 'w') as f:
        f.write(c_array)
    print(f"[Encode] C array written to: {output_path} ({len(compressed)} bytes)")

# Run the pipeline
if __name__ == '__main__':
    # Extract and decompress original HTML
    original_bytes = extract_first_block(header_file)
    decompress_and_save_html(original_bytes, html_output)

    print("\n[Edit] You can now modify the HTML in:", html_output)
    input("[Wait] Press Enter after editing and saving your modified HTML...")

    # Recompress and output as C array
    reencode_html_to_c_array(modified_html_input, c_array_output, 'index_ov2640_html_gz')
