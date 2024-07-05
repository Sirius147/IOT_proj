from flask import Flask, render_template, request, redirect, url_for
import sqlite3
from datetime import datetime

app = Flask(__name__)

def init_db():
    with sqlite3.connect('attendance.db') as conn:
        conn.execute('''
            CREATE TABLE IF NOT EXISTS attendance (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                name TEXT NOT NULL,
                date TEXT NOT NULL
            )
        ''')

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/mark_attendance', methods=['POST'])
def mark_attendance():
    name = request.form.get('name')
    date = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    with sqlite3.connect('attendance.db') as conn:
        conn.execute('INSERT INTO attendance (name, date) VALUES (?, ?)', (name, date))
    return redirect(url_for('attendance_list'))

@app.route('/attendance_list')
def attendance_list():
    with sqlite3.connect('attendance.db') as conn:
        cursor = conn.execute('SELECT name, date FROM attendance')
        attendance_records = cursor.fetchall()
    return render_template('attendance.html', records=attendance_records)

if __name__ == '__main__':
    init_db()
    app.run(host='0.0.0.0', port=5000)

