import re
import sqlite3
import os
import sys

content = None

def remove_shit(temp):
    if "<p>" in temp:
        temp = temp.replace("<p>","")
        temp = temp.replace("</p>","")
    if "<i>" in temp:
        temp = temp.replace("<i>","")
        temp = temp.replace("</i>","")
    if "<strong>" in temp:
        temp = temp.replace("<strong>","")
        temp = temp.replace("</strong>","")
    if "\t" in temp:
        temp = temp.replace("\t","")
    if "<img" in temp:
        i1 = temp.index("<img")
        i2 = temp.find(">",i1)
        sub = temp[i1:i2]
        temp = temp.replace(sub,"")
    if "<br>" in temp:
        temp = temp.replace("<br>","")
    if "<b>" in temp:
        temp = temp.replace("<b>","")
        temp = temp.replace("</b>","")
    if "<div class=\"qtext\">" in temp:
        temp = temp.replace("<div class=\"qtext\">","")
        temp = temp.replace("</div>","")
    return temp

def return_matches_file(filename, pattern):
    global content
    with open(filename, 'r',encoding="UTF-8") as file:
        content = file.read()
    c = 1
    matches = []
    print(len(content))
    #9692647

    while True and c!=384:
        print("küsimus: ",c)
        match = re.search(pattern, content, re.DOTALL)
        if match:
            # Remove the match
            content = content[:match.start()] + content[match.end():]
            #print(match.group(1))
            matches.append(match.group(1))
            c+=1
        else:
            print("exiting")
            break

    return matches

def return_matches_string(content, pattern):
    match = re.search(pattern, content, re.DOTALL)
    if match:
        return match.group(1)
    else:
        return -1
    
    
filename = "out_data.html"
db = "data.db"
of = "out_data2.html"

html_pattern = r"(?:(?:\n|.)*?)((?:<table class=\"answer\">)(?:\n|.)*?<\/table>(?:\n|.)*?<div class=\"qtext\">(?:\n|.)*?</div>)"
question_pattern = r"(?:(?:\n|.)*?)((?:<div class=\"qtext\")(?:\n|.)*?<\/div>)"
question_pattern_1 = r"(?:(?:\n|.)*?)((?:<div class=\"qtext\")(?:\n|.)*?<\/div>)"
question_pattern_2 = r"(?:(?:\n|.)*?)((?:<p>)(?:\n|.)*?<\/p>)"
question_pattern_3 = r"(?:(?:\n|.)*?)((?:<table)(?:\n|.)*?<\/table>)"


current_dir = os.path.dirname(os.path.realpath(__file__))

file_path = os.path.join(current_dir, filename)
db_path = os.path.join(current_dir, db)
outfile = os.path.join(current_dir,of)

html = return_matches_file(file_path,html_pattern)
questions_bad = []
questions = []

print(len(content))

w = "getting question from: "
c = 1
for  i in range(len(html)):
    print(w,c)
    temp = return_matches_string(html[i],question_pattern)
    #print(temp)
    questions_bad.append(temp)
    c+=1

    
w = "filtering question from: "
c = 1
for  i in range(len(questions_bad)):
    try:
        questions_bad[i] = questions_bad[i].replace(return_matches_string(questions_bad[i],question_pattern_3),"")
    except:
        pass
    
    temp = return_matches_string(questions_bad[i],question_pattern_2)
    if temp == -1:
        temp = questions_bad[i]
    try:
        temp = remove_shit(temp)
    except:
        print(questions_bad[i])
        sys.exit()
    print(w,c)
    questions.append(temp)
    c+=1

conn = sqlite3.connect(db_path)
cursor = conn.cursor()

#start fucking w the sql table
cursor.execute('''CREATE TABLE IF NOT EXISTS questions
                  (id INTEGER PRIMARY KEY, question TEXT, content TEXT)''')

w = "uploading to db: "
c = 0
for i in range(len(questions_bad)):
    #print(questions[i])
    print(w,i)
    cursor.execute("INSERT INTO questions (question, content) VALUES (?, ?)", (questions[i],html[i]))
    c+=1

conn.commit()
conn.close()

print(len(content))


"""with open(outfile,"w",encoding="UTF-8") as f:
    f.write(content)"""