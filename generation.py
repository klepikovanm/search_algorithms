import csv
import random

random.seed(1)

countries = [
    "Russia", "Germany", "Czech Republic", "Spain", "Italy", "France",
    "Portugal", "Netherlands", "Belgium", "Turkey", "Poland", "Canada"
]

cities = {
    "Russia": ["Moscow", "Saint Petersburg", "Yekaterinburg", "Krasnodar", "Kazan"],
    "Germany": ["Berlin", "Munich", "Dortmund", "Leipzig", "Hamburg"],
    "Czech Republic": ["Prague", "Brno", "Ostrava", "Pilsen", "Liberec"],
    "Spain": ["Madrid", "Barcelona", "Seville", "Valencia", "Bilbao"],
    "Italy": ["Rome", "Milan", "Naples", "Turin", "Florence"],
    "France": ["Paris", "Marseille", "Lyon", "Lille", "Nice"],
    "Portugal": ["Lisbon", "Porto", "Braga", "Coimbra", "Faro"],
    "Netherlands": ["Amsterdam", "Rotterdam", "Eindhoven", "Utrecht", "The Hague"],
    "Belgium": ["Brussels", "Antwerp", "Ghent", "Bruges", "Liège"],
    "Turkey": ["Istanbul", "Ankara", "Izmir", "Bursa", "Antalya"],
    "Poland": ["Warsaw", "Krakow", "Gdansk", "Wroclaw", "Poznan"],
    "Canada": ["Toronto", "Montreal", "Vancouver", "Ottawa", "Calgary"]
}

club_names = [
    "Altair", "Vector", "Horizon", "Spectrum", "Forward", "Orion",
    "Peregrine", "Titan", "Storm", "Crystal", "Torch", "Meteor",
    "Vortex", "Start", "Progress", "Leader", "Vanguard", "Monolith",
    "Energy", "Impulse", "Tornado", "Union", "Olympus", "Flame",
    "Dawn", "North", "Jupiter", "Cascade", "Sunrise", "Storm Petrel",
    "Sunrise", "Lightning", "Frigate", "Granite", "Pulse", "Magnet",
    "Contour", "Rocket", "Phoenix", "Pioneer", "Legion", "Master",
    "Record", "Typhoon", "Barrier", "Optima", "Everest", "Drive"
]

first_names = [
    "Alexander", "Sergey", "Dmitry", "Ivan", "Andrey", "Stepan",
    "Mikhail", "Nikolai", "Evgeny", "Vladimir", "Pavel", "Kirill"
]

middle_names = [
    "Ivanovich", "Petrovich", "Sergeyevich", "Alekseyevich", "Dmitrievich", "Antonovich",
    "Nikolaevich", "Andreyevich", "Vladimirovich", "Mikhailovich", "Evgenyevich", "Ilyich"
]

last_names = [
    "Ivanov", "Petrov", "Sidorov", "Kuznetsov", "Smirnov", "Korytov",
    "Vasnetsov", "Vasilyev", "Novikov", "Lazarev", "Morozov", "Kozlov"
]

def generate_coach_fio():
    return f"{random.choice(last_names)} {random.choice(first_names)} {random.choice(middle_names)}"

def generate_record():
    country = random.choice(countries)
    club_name = random.choice(club_names)
    city = random.choice(cities[country])
    year = random.randint(1920, 2025)
    coach = generate_coach_fio()
    points = random.randint(0, 100)
    return [country, club_name, city, year, coach, points]

rows_count = 1000000

with open("football_teams.csv", "w", newline="", encoding="utf-8-sig") as f:
    writer = csv.writer(f, delimiter=';')
    writer.writerow(["Country", "Club Name", "City", "Year", "Head Coach Full Name", "Points Scored"])
    for _ in range(rows_count):
        writer.writerow(generate_record())

print("football_teams.csv file successfully created.")