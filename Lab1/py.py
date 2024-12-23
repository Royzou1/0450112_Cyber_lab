import numpy as np

# Define the dataset (assuming it is loaded as a dictionary)
data = {
    "traces": [
        [844590,850123,761909,535835,741587,830698,849488,850527,859432,859807,856437,860136,860611,860685,857331,818945,833177,839358,845144,842918,841691,837621,839111,739393,857127,847582,845095,852380,817965,832359,818085,622645,796253,763486,777404,799541,795053,745038,751062,833943,698623,711815,671456,659424,743289,744682,671266,766195,776399,815970,762830,800752,834814,829102,797045,814087,847369,847676,837832,838848,848992,848555,849562,850754,825766,828260,833058,851145,828993,829896,824944,825818,830705,831947,804811,830014,832004,829404,826003,831185,828809,817869,826333,832268,832263,828960,825674,833604,830761,824734,830380,831168,834044,834766,834075,828353,829692,830717,830150,829558],[841171,782581,804768,779866,824534,845392,853529,855751,851695,856266,852913,790493,799019,819866,830864,833688,832803,834858,830456,851357,853993,835751,852281,855733,855142,850545,853943,853949,845458,851437,841301,852321,852395,851158,853746,856092,847195,854315,846887,836055,692415,763293,850416,807930,818012,853803,850224,786692,755032,833370,768729,803025,785191,796226,817709,774509,821250,830687,748771,748561,811731,847262,850162,848253,844297,847562,849070,846269,847080,851593,846061,852006,850247,850815,814793,850490,848421,842530,851537,846982,838690,836765,835146,839610,836081,836273,823759,851246,845281,852978,848086,852370,848225,856386,850390,852579,850120,853898,851745,820150],[700240,684444,706816,731425,770940,772943,789088,790047,784373,785306,784774,758832,779405,827884,839131,823091,825480,830664,831110,627592,593605,666518,729045,687964,597659,612820,629528,599162,660978,764240,687957,657199,591139,594584,599436,587896,617363,731504,702732,761075,730836,653517,651517,617227,622579,613348,697020,707101,788215,807974,816533,823629,839190,837708,841902,838930,834993,840009,838983,760201,762095,790983,793244,761719,790704,798040,804114,800401,799013,795582,798480,796631,804252,786807,766854,805157,808233,797732,807323,788402,809251,776062,799531,800629,790198,810683,809280,813443,815923,811963,812914,814139,814987,810059,814139,813673,814431,816344,815918,813631],[840009,786395,787242,789007,837682,816710,808931,809805,814020,813739,817405,804231,758718,791341,809783,814015,813603,815899,813799,821650,814307,812370,845114,848017,849146,845772,850213,848220,846481,850390,852703,850151,850619,852216,852026,845081,850518,849426,847808,829745,743935,761039,780454,673637,737992,713218,690988,654441,608338,747561,817638,817980,820079,847851,847042,845833,848385,848522,847472,846206,846832,832402,832942,837243,848785,816616,826693,787194,823269,838333,839660,841708,848991,843002,813428,846176,799959,803429,773098,820834,836898,792072,800525,805635,812383,812872,832648,833650,830359,833252,832405,834021,830880,833304,835199,830517,832718,831548,807016,829155],[784416,795514,651019,717316,734207,775323,804688,812061,685811,717148,773709,520825,702480,688475,663918,679603,788143,806864,808538,721993,777804,779020,772273,782044,783378,697747,806385,828749,840233,836190,642435,756929,827150,837098,840223,837408,835153,840787,826050,835514,840084,841440,840645,840938,837973,842090,840278,824666,834813,837875,833937,829400,842408,846194,842797,844960,845171,845984,844268,844815,841255,845707,845011,844621,845179,846359,844728,845766,845958,845488,844250,844245,835507,842670,661379,772650,750602,752699,695557,812502,841725,841846,835545,842212,835924,835897,716049,698932,612612,588547,628951,632635,703453,719802,745618,783148,824420,764829,791435,818718],[793904,843278,660550,683166,723903,695712,747232,726411,521515,624488,625394,653320,621954,622159,754195,737782,730430,741445,753975,831298,828925,839800,717472,817710,809829,829366,824262,743036,820019,835160,834940,817667,804398,840481,840651,840934,840003,840021,824161,838004,837320,840894,840897,829032,837054,762801,831673,739866,793448,776514,804517,839975,813861,797179,797026,799110,769164,662306,741310,788613,714447,751394,760275,696053,715342,661131,716531,802882,736034,701939,731580,776884,753353,764384,701622,720935,722988,727429,714406,733411,728161,724447,736675,731356,737773,706523,712824,738658,730927,727678,734095,696719,719322,734799,724727,679780,818171,835801,804530,822313],[834265,840277,736697,613879,740652,719385,689092,721880,715025,553988,622711,614097,686262,647824,750859,702957,714442,716541,745578,776143,813330,743903,798580,744359,634698,562594,719401,827545,837612,837622,842088,842886,840812,838462,843899,839410,842825,840477,833494,821673,704718,743344,773638,730083,773007,645294,650386,667830,633544,730560,785318,787005,742052,778618,784398,798268,799996,792503,808020,803925,807498,796173,798572,801917,783539,753320,725991,720536,726445,746859,722916,731674,728115,808894,796144,832656,828837,835201,833923,834072,835094,819186,834705,783174,832878,819505,826358,831813,830479,835160,813792,814707,819093,823708,824002,826473,824650,820722,820284,819683],[793572,679791,694801,572073,733514,693085,779970,781141,711536,696490,779337,663430,439689,596927,720945,751091,712481,778705,809228,641115,773852,780731,613325,602039,632201,541967,535418,561074,588848,645575,650823,656516,624539,614538,662102,618772,633682,625189,617773,616622,605230,563742,641836,573453,557830,590861,648349,581116,578139,591103,731203,667248,601610,567292,617161,584013,559484,632000,697697,634641,583726,567761,579717,506347,556869,587017,651149,568916,625753,582888,545849,642989,581572,564495,672788,635794,681336,571418,554924,557765,570655,562087,675934,690622,694889,736395,762142,693532,740681,812816,801207,811727,829030,829200,830458,830749,828483,797872,829056,827359]
    ],
    "labels": [
        "https://www.google.com","https://www.google.com","https://www.google.com","https://www.google.com","https://www.nytimes.com","https://www.nytimes.com","https://www.nytimes.com","https://www.nytimes.com"]
}

# Function to calculate statistics for each trace
def analyze_trace(trace):
    trace = np.array(trace)
    min_value = np.min(trace)
    max_value = np.max(trace)
    median_value = np.median(trace)
    mean_value = np.mean(trace)
    variance_value = np.var(trace)
    
    return {
        "min": min_value,
        "max": max_value,
        "median": median_value,
        "mean": mean_value,
        "variance": variance_value
    }

# Analyze all traces and output results
results = []
for idx, trace in enumerate(data["traces"]):
    stats = analyze_trace(trace)
    results.append({
        "trace_id": idx + 1,
        "label": data["labels"][idx],
        "statistics": stats
    })

# Print the results for each trace
for result in results:
    print(f"Trace {result['trace_id']} (Label: {result['label']}):")
    print(f"  Min: {result['statistics']['min']}")
    print(f"  Max: {result['statistics']['max']}")
    print(f"  Median: {result['statistics']['median']}")
    print(f"  Mean: {result['statistics']['mean']}")
    print(f"  Variance: {result['statistics']['variance']}")
    print("\n")
