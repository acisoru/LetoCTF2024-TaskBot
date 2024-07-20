package main

import (
	"bufio"
	"fmt"
	"math"
	"math/rand"
	"net"
	"os"
	"strconv"
	"strings"
	"time"

	log "github.com/sirupsen/logrus"
)

const float64EqualityThreshold = 1

func almostEqual(a, b float64) bool {
	log.Info(math.Abs(a - b))
	return math.Abs(a-b) <= float64EqualityThreshold
}

func GenerateEnemyXY(r *rand.Rand) (float64, float64) {
	EnemyXStart := 100.0 + float64(r.Intn(265))
	EnemyYStart := 100.0 + float64(r.Intn(265))
	Radius := math.Sqrt(math.Pow(EnemyXStart, 2) + math.Pow(EnemyYStart, 2))
	if Radius <= 367 {
		return EnemyXStart, EnemyYStart
	} else {
		return GenerateEnemyXY(r)
	}
}

func GenerateEnemyAngle(r *rand.Rand, x, y float64) float64 {
	if r.Intn(2) == 0 {
		return math.Atan(y/x)*180/math.Pi + float64(180) + float64(r.Intn(14))
	} else {
		return math.Atan(y/x)*180/math.Pi + float64(180) - float64(r.Intn(14))
	}
}

func GenerateEnemyAcceleration(r *rand.Rand) float64 {
	return r.Float64() * 5
}

func GenerateEnemyVelocity(r *rand.Rand) float64 {
	return r.Float64() * 5
}

type Task struct {
	EnemyXStart       float64
	EnemyYStart       float64
	EnemyAngle        float64
	EnemyVelocity     float64
	EnemyAcceleration float64
}

func GenerateTask() (string, Task) {
	r := rand.New(rand.NewSource(time.Now().UnixNano()))
	task := Task{}
	//r.Intn(100) -> 0 <= n < 100
	//GRAVITY IS FIXED, 9.81
	task.EnemyXStart, task.EnemyYStart = GenerateEnemyXY(r)
	task.EnemyAngle = GenerateEnemyAngle(r, task.EnemyXStart, task.EnemyYStart)
	task.EnemyVelocity = GenerateEnemyVelocity(r)
	task.EnemyAcceleration = GenerateEnemyAcceleration(r)
	taskDescription := fmt.Sprintf(`Ο ανιχνευτής μας λέει ότι οι εχθροί θα κινηθούν ως εξής:
Sambuca μέγεθος 1. Δεν χρειάζεται να πυροβολείτε τέλεια.
Ο καταπέλτης και η σαμπούκα βρίσκονται στο ίδιο ύψος.
Είναι γνωστό ότι η κατεύθυνση της αρχικής ταχύτητας συμπίπτει με την κατεύθυνση της επιτάχυνσης.
Η βαρύτητα είναι 9,81.
Η ταχύτητα του βλήματος καταπέλτη είναι 60 m/s.
Ο εχθρός βρίσκεται σε αυτό το σημείο: %v %v
Ο εχθρός κινείται προς αυτή την κατεύθυνση: %v
Ο εχθρός κινείται τόσο γρήγορα: %v m/s
Ο εχθρός επιταχύνεται: %v m/s^2

Πες μου πού να πυροβολήσω από τον καταπέλτη;
Γράψτε τη γωνία ανύψωσης και την κατεύθυνση της φωτιάς σε μοίρες.
`, task.EnemyXStart, task.EnemyYStart,
		task.EnemyAngle, task.EnemyVelocity, task.EnemyAcceleration)
	return taskDescription, task
}

func to_radians(x float64) float64 {
	return x * math.Pi / 180
}

func CheckTaskResult(task Task, alpha, theta float64) bool {
	log.Info(to_radians(alpha), to_radians(theta))
	time := (2 * 60 * math.Sin(to_radians(alpha))) / 9.81
	leftX := (60 * 60 * math.Sin(to_radians(2*alpha)) * math.Cos(to_radians(theta))) / 9.81
	rightX := (task.EnemyVelocity*time+task.EnemyAcceleration*math.Pow(time, 2)/2)*math.Cos(to_radians(task.EnemyAngle)) + task.EnemyXStart
	leftY := (60 * 60 * math.Sin(to_radians(2*alpha)) * math.Sin(to_radians(theta))) / 9.81
	rightY := (task.EnemyVelocity*time+task.EnemyAcceleration*math.Pow(time, 2)/2)*math.Sin(to_radians(task.EnemyAngle)) + task.EnemyYStart
	if almostEqual(leftX, rightX) && almostEqual(leftY, rightY) {
		return true
	}
	return false
}

func handleConnection(c net.Conn) {
	log.Info(fmt.Sprintf("Serving %v\n", c.RemoteAddr()))
	for i := 1; i <= 100; i++ {
		taskDescription, task := GenerateTask()
		if i == 1 {
			taskDescription = `
░█▀▀▀█ ░█▀▀▀█ ─█▀▀█ ░█▀▀▄ ─█▀▀█   ░█▀▀▀█ ▀█▀ ░█▀▀█ ─█▀▀█ ░█─▄▀ ░█─░█ ░█▀▀▀█
░█──░█ ─▀▀▀▄▄ ░█▄▄█ ░█─░█ ░█▄▄█   ─▀▀▀▄▄ ░█─ ░█▄▄▀ ░█▄▄█ ░█▀▄─ ░█─░█ ─▄▄▄▀▀
░█▄▄▄█ ░█▄▄▄█ ░█─░█ ░█▄▄▀ ░█─░█   ░█▄▄▄█ ▄█▄ ░█─░█ ░█─░█ ░█─░█ ─▀▄▄▀ ░█▄▄▄█

` + taskDescription
		}
		c.Write([]byte(taskDescription))
		log.Info(fmt.Sprintf("%v sent task\n", c.RemoteAddr()))
		message, err := bufio.NewReader(c).ReadString('\n')
		if err != nil {
			log.Info(fmt.Sprintf("%v error in reading answer\n", c.RemoteAddr()))
			c.Write([]byte(err.Error()))
			c.Close()
			return
		}
		message = strings.ToLower(strings.TrimSpace(message))
		messageSplitted := strings.Split(message, " ")
		alphaString := messageSplitted[0]
		thetaString := messageSplitted[1]
		alpha, err := strconv.ParseFloat(alphaString, 64)
		if err != nil {
			log.Info(fmt.Sprintf("%v error in parsing float\n", c.RemoteAddr()))
			c.Write([]byte(err.Error()))
			c.Close()
			return
		}
		theta, err := strconv.ParseFloat(thetaString, 64)
		if err != nil {
			log.Info(fmt.Sprintf("%v error in parsing float\n", c.RemoteAddr()))
			c.Write([]byte(err.Error()))
			c.Close()
			return
		}
		if !CheckTaskResult(task, alpha, theta) {
			log.Info(fmt.Sprintf("%v failed calculation\n", c.RemoteAddr()))
			c.Write([]byte("errors in calculation\n"))
			c.Close()
			return
		} else {
			log.Info(fmt.Sprintf("%v successfully hit!\n", c.RemoteAddr()))
			// c.Write([]byte("hit\n"))
			// time.Sleep(500)
		}
	}
	log.Info(fmt.Sprintf("%v winned a flag\n", c.RemoteAddr()))
	c.Write([]byte(FLAG))
	c.Close()
}

var LISTENADDRESS = os.Getenv("OSADA_SIRAKUZ_LISTEN_ADDRESS")
var FLAG = os.Getenv("OSADA_SIRAKUZ_FLAG")

func main() {
	log.SetFormatter(&log.TextFormatter{
		DisableColors: false,
		FullTimestamp: true,
	})
	log.SetLevel(log.TraceLevel)
	log.SetOutput(os.Stdout)
	log.Info(fmt.Sprintf("Starting server on addr: %v...", LISTENADDRESS))

	ln, _ := net.Listen("tcp", LISTENADDRESS)
	defer ln.Close()
	for {
		conn, err := ln.Accept()
		if err != nil {
			log.Warn(err)
		}
		go handleConnection(conn)
	}
}
