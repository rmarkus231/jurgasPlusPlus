from bs4 import BeautifulSoup as BS

with open("converter/kysimused.bin","r", encoding="utf-8",errors="ignore") as file:
    soup = BS(file, 'html.parser')
    xml_content = soup.prettify()


with open("output.xml", "w",encoding="utf-8",errors="ignore") as xml_file:
    xml_file.write(xml_content)