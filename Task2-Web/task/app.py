from lxml import etree
from flask import Flask, render_template, Response, make_response, request
from markupsafe import escape

app = Flask(__name__)


@app.route("/", methods=['GET', 'POST'])
def home():
    result = ''
    if request.method == 'POST':
        xpath_query = request.form.get('search')
        if xpath_query:
            try:
                tree = etree.parse('data/secret_doc.xml')
                test = tree.xpath("/root/item/name[text()='" + xpath_query + "']")
                if len(test) == 0:
                    result = 'Ничего не найдено'
                else:
                    result = 'Данные переданы для модерации'
            except Exception as e:
                result = f"Error!"
    return render_template('index.html', data=result)


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=8001)
