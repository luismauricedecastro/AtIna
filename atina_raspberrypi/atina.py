import serial
import time

from flask import Flask

print("Iniciando...")

ser = serial.Serial('/dev/ttyACM0', 57600, timeout=1.0)
time.sleep(3)

print("Serial ok...")

app = Flask(__name__)

@app.route("/")
def index():
	return menu()

@app.route("/forward")
def forward():
	try:
		ser.write("FM\n".encode())
		return menu()
	except:
		return error_page()

@app.route("/ff")
def forward_fast():
	try:
		ser.write("FF\n".encode())
		return menu()
	except:
		return error_page()

@app.route("/back")
def back():
	try:
		ser.write("BM\n".encode())
		return menu()
	except:
		return error_page()

@app.route("/bf")
def back_fast():
	try:
		ser.write("BF\n".encode())
		return menu()
	except:
		return error_page()

@app.route("/left")
def left():
	try:
		ser.write("LM\n".encode())
		return menu()
	except:
		return error_page()
		
@app.route("/right")
def right():
	try:
		ser.write("RM\n".encode())
		return menu()
	except:
		return error_page()
	
@app.route("/yes")
def yes():
	try:
		ser.write("y\n".encode())
		return menu()
	except:
		return error_page()
	
@app.route("/no")
def no():
	try:
		ser.write("n\n".encode())
		return menu()
	except:
		return error_page()
		
def menu():
	return """
	
<html>
	<head>
		<meta charset="utf-8">
		<meta name="viewport" content="width=device-width, initial-scale=1">
		
		<link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.2/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-T3c6CoIi6uLrA9TneNEoa7RxnatzjcDSCmG1MXxSR1GAsXEV/Dwwykc2MPK8M2HN" crossorigin="anonymous">
	
		<style>
			.atbutton {
				height: 100px;
				width: 100px;
			}
		</style>
	</head>
	
	<body>
	<div class='container' style='text-align: center;'>
		<br>
		<h1><a href='/'>AtIna OS version 1.0</a></h1>

		<hr>
		
		<table class='table' style='text-align: center;'>
			<tr>
				<td></td>
				<td><a class='btn btn-primary atbutton' href='/ff'>FF</a></td>
				<td></td>
			</tr>
			
			<tr>
				<td></td>
				<td><a class='btn btn-primary atbutton' href='/forward'>F</a></td>
				<td></td>
			</tr>
			
			<tr>
				<td><a class='btn btn-primary atbutton' href='/left'>L</a></td>
				<td></td>
				<td><a class='btn btn-primary atbutton' href='/right'>R</a></td>
			</tr>
			
			<tr>
				<td></td>
				<td><a class='btn btn-primary atbutton' href='/back'>B</a></td>
				<td><a class='btn btn-primary atbutton' href='/yes'>Y</a></td>
			</tr>
			
			<tr>
				<td></td>
				<td><a class='btn btn-primary atbutton' href='/bf'>BF</a></td>
				<td><a class='btn btn-primary atbutton' href='/no'>N</a></td>
			</tr>
		
		</table>
	</div>
	</body>
</html>
	"""
	
def error_page():
	return """
	
<html>
	<head>
		<meta charset="utf-8">
		<meta name="viewport" content="width=device-width, initial-scale=1">
		
		<link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.2/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-T3c6CoIi6uLrA9TneNEoa7RxnatzjcDSCmG1MXxSR1GAsXEV/Dwwykc2MPK8M2HN" crossorigin="anonymous">
	
		<style>
			.atbutton {
				height: 100px;
				width: 100px;
			}
		</style>
	</head>
	
	<body>
	<div class='container' style='text-align: center;'>
		<br>
		<h1><a href='/'>AtIna OS version 1.0</a></h1>

		<hr>
			<h3>Error</h3>
		<hr>
		
		<a class='btn btn-primary atbutton' href='/'>Inicio</a>
	</div>
	</body>
</html>
	"""
	
print("Iniciando host...")

try:
	app.run(host = "0.0.0.0")
except KeyboardInterrupt:
	print("Cerrando serial")
	ser.close()
