FROM golang:1.18-alpine AS build_base

RUN apk add --no-cache git

# Set the Current Working Directory inside the container
WORKDIR /tmp/osada_sirakuz

# We want to populate the module cache based on the go.{mod,sum} files.
COPY ./go.mod .
COPY ./go.sum .

RUN go mod download

COPY ./main.go .

# Build the Go app
RUN go build -o ./out/main .

# Start fresh from a smaller image
FROM alpine

ENV OSADA_SIRAKUZ_LISTEN_ADDRESS ":8181"
ENV OSADA_SIRAKUZ_FLAG "n0l1_7urb4r3_c1rcul05_m305"

RUN apk add ca-certificates

COPY --from=build_base /tmp/osada_sirakuz/out/main /app/main

# This container exposes port 8080 to the outside world
EXPOSE 8181

# Run the binary program produced by `go install`
CMD ["/app/main"]
