import google.generativeai as genai
import os

api_key = 'AIzaSyCBvgGAav5RbTUVYK4w_wS04vueP5nX3Ok'
genai.configure(api_key = api_key)

model = genai.GenerativeModel('gemini-pro')
response = model.generate_content('你好')

print(response.text)