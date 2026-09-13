htmldata = ""

with open("ac.html", "r") as ac:
    htmldata = ac.read().replace('"', '\\"').split("\n")
    ac.close()

open("webpage.h", "w").close()
with open("webpage.h", "a") as h:
    h.write("const char* webpage = ")

    for line in htmldata:
        h.write(f" \"{line}\"\n")

    h.write(";")
    h.close();
