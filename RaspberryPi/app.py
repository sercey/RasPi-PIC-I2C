from flask import Flask, jsonify
import smbus
import time

bus = smbus.SMBus(1)
slave1 = 0x38

app = Flask(__name__)

@app.route('/')
def index():
    return "Hosgeldiniz!"

def i2c_veri_yaz(pin_adres, durum):
    try:
        bus.write_byte_data(slave1, pin_adres, durum)
        return True
    except:
        return False


def i2c_veri_oku(pin_adres):
    _ret = 0
    try:
        time.sleep(0.1)
        bus.write_byte(slave1, pin_adres)
        time.sleep(0.1)
        _ret = bus.read_byte(slave1)
        return _ret
    except:
        return -1

@app.route('/i2c-api/pin_durum/<int:pin_id>', methods = ['GET'])
def pin_durum(pin_id):
    _success = "";
    _response = "";
    
    _veri_oku = i2c_veri_oku(pin_id)
    if(_veri_oku != -1):
        _success = True
        _response = _veri_oku
    else:
       _success = False
    
    return jsonify(success = _success, response = _response)

@app.route('/i2c-api/pin_ac/<int:pin_id>', methods = ['GET'])
def pin_ac(pin_id):
    _success = "";
    _response = "";
    
    if(i2c_veri_yaz(pin_id, 1) == True):
        _success = True
        _veri_oku = i2c_veri_oku(pin_id)
        
        if(_veri_oku != -1):
            _response = _veri_oku
            
    else:
        _success = False
    
    return jsonify(success = _success, response = _response)
    
@app.route('/i2c-api/pin_kapat/<int:pin_id>', methods = ['GET'])
def pin_kapat(pin_id):
    _success = "";
    _response = "";
    
    if(i2c_veri_yaz(pin_id, 0) == True):
        _success = True
        _veri_oku = i2c_veri_oku(pin_id)
        
        if(_veri_oku != -1):
            _response = _veri_oku
            
    else:
        _success = False
    
    return jsonify(success = _success, response = _response)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=555, debug=True)