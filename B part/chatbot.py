import re
from datetime import datetime

rules = [
    (r"my name is (\w+)",     "Nice to meet you, {1}!"),
    (r"how are you",           "I am fine! Thanks for asking."),
    (r"who created you",       "I was created using Python."),
    (r"what is (your )?name",  "I am a simple Python chatbot."),
    (r"what is current time",  "__TIME__"),
    (r"what is current date",  "__DATE__"),
    (r"hello|hi|hey",          "Hello! How can I help you?"),
    (r"bye|goodbye",           "__BYE__"),
]

def respond(user_input):
    text = user_input.strip().lower()
    for pattern, reply in rules:
        m = re.search(pattern, text)
        if m:
            if reply == "__TIME__":
                return f"Current time: {datetime.now().strftime('%H:%M:%S')}"
            if reply == "__DATE__":
                return f"Today's date: {datetime.now().strftime('%d-%m-%Y')}"
            if reply == "__BYE__":
                return None
            for i, g in enumerate(m.groups(), 1):
                if g:
                    reply = reply.replace(f"{{{i}}}", g.capitalize())
            return reply
    return "Sorry, I don't understand. Please try another question."

print("=" * 45)
print("           SIMPLE PYTHON CHATBOT")
print("=" * 45)
print("CHATBOT: Hello! Type 'bye' to exit.")
print("-" * 45)

while True:
    user_input = input("You      : ")
    answer = respond(user_input)
    if answer is None:
        print("Chatbot  : Goodbye! Have a nice day.")
        print("=" * 45)
        break
    print(f"Chatbot  : {answer}")
