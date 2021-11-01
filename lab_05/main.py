# Вариант 16
# Задание варианта: Доставка сообщений выполняется с регулярным интервалом.
# Интервал и тело сообщения, имя файла для прикрепления (опционально) вводятся с клавиатуры.

# Открытый smtp-сервер не найден, поэтому использую просто бесплатный smtp.gmail.com.

# Google не разрешит вход через smtplib, поскольку этот тип входа помечен как «менее безопасный».
# Чтобы решить эту проблему, перейдите на страницу https://myaccount.google.com/lesssecureapps
# и разрешите менее безопасные приложения.
# По умолчанию письма отправляются с созданной ранее почты, которая настроена.
# Логин и пароль можно увидеть в коде

import smtplib
from email.mime.application import MIMEApplication
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
import time
from datetime import datetime, timedelta
import threading

def send_mail(TO, subj, body, FROM = "test.pvs.bmstu@gmail.com", PASSWORD="Tedlg12&", server_host="smtp.gmail.com", port=587, interval=0, times=1):

    msg = fill_msg(TO, FROM, subj, body)

    server = smtplib.SMTP(server_host, port)
    server.starttls()
    server.login(msg['From'], PASSWORD)

    for i in range(times):
        server.sendmail(msg['From'], msg['To'], msg.as_string())
        time.sleep(interval)
        # print(f"email to {msg['to']} delivered")
    server.quit()


def fill_msg(TO, FROM, subj, body):

    msg = MIMEMultipart()
    msg['To'] = TO
    msg['From'] = FROM

    subj = subj
    body = body
    msg['Subject'] = subj

    msg.attach(MIMEText(body))

    return msg


def main():
    Choice = None
    while Choice != '0':
        print("""
                Выберите режим.
            1 - тестовый режим (настроенная по умолчанию почта)
            2 - реальный режим (своя почта)
            0 - выход
        """)
        Choice = input("Выбор: ")

        NextChoice = None
        if Choice == '1':
            while NextChoice != '0':
                print(""" 
                        Тестовый режим.
                    1 - создать и отправить письмо
                    0 - назад 
                """)
                NextChoice = input("Выбор: ")

                if NextChoice == '1':
                    TO = input("Адрес получателя: ")
                    subj = input("Тема письма: ")
                    body = input("Текст письма: ")
                    is_interval = None
                    while is_interval not in ("NO",  "no", 'n', "YES", "yes", 'y'):
                        is_interval = input("Хотите отправлять письмо с интервалом? (yes/no): ")
                        if is_interval in ("NO",  "no", 'n'):
                            send_mail(TO=TO, subj=subj, body=body)
                        elif is_interval in ("YES", "yes", 'y'):
                            interv = int(input("Интервал в секундах: "))
                            interval = timedelta(seconds=interv)
                            times = int(input("Количество отправок: "))
                            thread = threading.Thread(target=send_mail, kwargs={'TO': TO,
                                                                                'subj': subj,
                                                                                'body': body,
                                                                                'interval': interval.total_seconds(),
                                                                                'times': times})
                            thread.start()

                elif NextChoice != '0':
                    print("Неверный ввод. Попробуйте снова.")

        elif Choice == '2':
            while NextChoice != '0':
                print(""" 
                        Реальный режим.
                    1 - создать и отправить письмо
                    0 - назад 
                """)
                NextChoice = input("Выбор: ")

                if NextChoice == '1':
                    FROM = input("Адрес отправителя: ")
                    PASSWORD = input("Пароль: ")
                    TO = input("Адрес получателя: ")
                    subj = input("Тема письма: ")
                    body = input("Текст письма: ")
                    is_interval = None
                    while is_interval not in ("NO",  "no", 'n', "YES", "yes", 'y'):
                        is_interval = input("Хотите отправлять письмо с интервалом? (yes/no): ")
                        if is_interval in ("NO",  "no", 'n'):
                            send_mail(TO=TO, FROM=FROM, PASSWORD=PASSWORD, subj=subj, body=body)
                        elif is_interval in ("YES", "yes", 'y'):
                            interv = int(input("Интервал в секундах: "))
                            interval = timedelta(seconds=interv)
                            times = int(input("Количество отправок: "))
                            thread = threading.Thread(target=send_mail,
                                                      kwargs={'TO': TO,
                                                              'subj': subj,
                                                              'body': body,
                                                              'FROM': FROM,
                                                              'PASSWORD': PASSWORD,
                                                              'interval': interval.total_seconds(),
                                                              'times': times})
                            thread.start()

                elif NextChoice != '0':
                    print("Неверный ввод. Попробуйте снова.")

        elif Choice != '0':
            print("Неверный ввод. Попробуйте снова.")
    print("finish")


if __name__ == '__main__':
    main()

