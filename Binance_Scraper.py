import requests
import json

#response = requests.get('https://api.binance.com/api/v1/ticker/allBookTickers')

# pairs[] was gathered from executing above line and then just copy pasted to a list for reusability.
# Will probably want to re-run this every 24hrs to check for new coins.
pairs = ['TRIGBNB', 'TRIGETH',  'TRIGBTC',  'LUNETH',   'LUNBTC', 'NAVBNB',    'NAVETH',   'NAVBTC',   'GTOBTC',
        'WINGSETH', 'WINGSBTC', 'EDOETH',   'EDOBTC',   'MCOBNB',   'BRDBNB',   'BRDETH',   'BRDBTC',   'WABIBNB',
        'NEBLBNB',  'NEBLETH',  'NEBLBTC',  'AIONBNB',  'AIONETH',  'AIONBTC',  'ELFETH',   'ELFBTC',   'XLMBNB',
        'OSTBNB',   'OSTETH',   'OSTBTC',   'ICXBNB',   'ICXETH',   'ICXBTC',   'GTOBNB',   'GTOETH',   'ADABTC',
        'WAVESBNB', 'WAVESETH', 'WAVESBTC', 'TNBETH',   'TNBBTC',   'LTCBNB',   'LTCUSDT',  'LTCETH',   'MANAETH',
        'WABIETH',  'WABIBTC',  'LENDBNB',  'LENDETH',  'LENDBTC',  'CNDBNB',   'CNDETH',   'CNDBTC',   'XZCBNB',
        'XLMETH',   'XLMBTC',   'CMTBNB',   'CMTETH',   'CMTBTC',   'PPTETH',   'PPTBTC',   'ADAETH',   'POEETH',
        'ADXBNB',   'ADXETH',   'ADXBTC',   'IOTABNB',  'DGDETH',   'DGDBTC',   'BCDETH',   'BCDBTC',   'GVTBTC',
        'MANABTC',  'FUELETH',  'FUELBTC',  'TNTETH',   'TNTBTC',   'LSKBNB',   'LSKETH',   'LSKBTC',   'BCCBNB',
        'XZCETH',   'XZCBTC',   'BTSBNB',   'BTSETH',   'BTSBTC',   'QSPBNB',   'QSPETH',   'QSPBTC',   'DLTBNB',
        'POEBTC',   'NEOBNB',   'NEOUSDT',  'GXSETH',   'GXSBTC',   'CDTETH',   'CDTBTC',   'GVTETH',   'RCNETH',
        'ARNETH',   'ARNBTC',   'BCPTBNB',  'BCPTETH',  'BCPTBTC',  'BATBNB',   'BATETH',   'BATBTC',   'VENBNB',
        'BCCUSDT',  'BCCETH',   'AMBBNB',   'AMBETH',   'AMBBTC',   'DLTETH',   'DLTBTC',   'WTCBNB',   'XRPBTC',
        'XMRETH',   'XMRBTC',   'RDNBNB',   'RDNETH',   'RDNBTC',   'NULSETH',  'NULSBTC',  'RCNBNB',   'VIBETH',
        'RCNBTC',   'NULSBNB',  'KMDETH',   'KMDBTC',   'VENETH',   'VENBTC',   'POWRBNB',  'YOYOBNB',  'OAXBTC',
        'BNBUSDT',  'STORJETH', 'STORJBTC', 'ENJETH',   'ENJBTC',   'MODETH',   'MODBTC',   'XRPETH',   'ENGETH',
        'YOYOETH',  'ARKETH',   'ARKBTC',   'POWRETH',  'POWRBTC',  'TRXETH',   'TRXBTC',   'HSRETH',   'SUBBTC',
        'VIBBTC',   'REQETH',   'REQBTC',   'EVXETH',   'EVXBTC',   'BTGETH',   'BTGBTC',   'ICNBTC',   'XVGETH',
        'DASHETH',  'DASHBTC',  'ASTETH',   'ASTBTC',   'BNTBTC',   'ZECETH',   'ZECBTC',   'DNTBTC',   'FUNETH',
        'ENGBTC',   'MTHETH',   'MTHBTC',   'ETCBTC',   'ETCETH',   'SNTBTC',   'EOSBTC',   'SUBETH',   'STRATBTC',
        'MTLETH',   'MTLBTC',   'MDAETH',   'MDABTC',   'SALTETH',  'SALTBTC',  'CTRETH',   'CTRBTC',   'WTCETH',
        'XVGBTC',   'LINKETH',  'LINKBTC',  'IOTAETH',  'IOTABTC',  'NEOETH',   'SNMETH',   'SNMBTC',   'BTCUSDT',
        'FUNBTC',   'KNCETH',   'KNCBTC',   'BQXETH',   'BQXBTC',   'SNGLSETH', 'SNGLSBTC', 'STRATETH', 'NEOBTC',
        'ZRXETH',   'ZRXBTC',   'OMGETH',   'OMGBTC',   'YOYOBTC',  'QTUMBTC',  'LRCETH',   'LRCBTC', 
        'WTCBTC',   'MCOBTC',   'ICNETH',   'MCOETH',   'DNTETH',   'OAXETH',   'HSRBTC',   'ETHUSDT', 
        'BNBETH',   'GASBTC',   'BCCBTC',   'BNTETH',   'SNTETH',   'EOSETH',   'QTUMETH',  '123456', 
        'BNBBTC',   'LTCBTC',   'ETHBTC'
         ]

# Gather last 4 hrs of data and write to respective files.
for pair in pairs[:]:
    response = requests.get('https://api.binance.com/api/v1/klines?symbol=' + pair + '&interval=1m')
    results = response.json()
    print('Writing to ' + pair + '_BINANCE_MINUTE_CANDLES.txt...')
    currentPairFile = open(pair + '_BINANCE_MINUTE_CANDLES.txt', 'w')
    currentPairFile.write("".join(str(results)))
    currentPairFile.close()

print("Scrape complete.")

